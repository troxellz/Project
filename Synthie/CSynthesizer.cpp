#include "pch.h"
#include "CSynthesizer.h"
#include "CToneInstrument.h"
#include "xmlhelp.h"
#include <string>
#include <algorithm>

using namespace std;

CSynthesizer::CSynthesizer(void)
{
    CoInitialize(NULL);

    m_channels = 2;
    m_sampleRate = 44100.;
    m_samplePeriod = 1 / m_sampleRate;
    m_time = 0;

    m_bpm = 120;
    m_secperbeat = 0.5;
    m_beatspermeasure = 2;
}

void CSynthesizer::Start(void)
{
    m_instruments.clear();
    m_currentNote = 0;
    m_measure = 0;
    m_beat = 0;
    m_time = 0;
}


bool CSynthesizer::Generate(double* frame)
{
    //
    // Phase 1: Determine if any notes need to be played.
    //

    while (m_currentNote < (int)m_notes.size())
    {
        // Get a pointer to the current note
        CNote* note = &m_notes[m_currentNote];

        // If the measure is in the future we can't play
        // this note just yet.
        if (note->Measure() > m_measure)
            break;

        // If this is the current measure, but the
        // beat has not been reached, we can't play
        // this note.
        if (note->Measure() == m_measure && note->Beat() > m_beat)
            break;

        //
        // Play the note!
        //

        // Create the instrument object
        CInstrument* instrument = NULL;
        if (note->Instrument() == L"ToneInstrument")
        {
            instrument = new CToneInstrument();
        }

        // Configure the instrument object
        if (instrument != NULL)
        {
            instrument->SetSampleRate(GetSampleRate());
            instrument->SetNote(note);
            instrument->SetBPM(m_bpm);
            instrument->Start();

            m_instruments.push_back(instrument);
        }

        if (note->Instrument() == L"reverb")
        {
            for (list<CInstrument*>::iterator node = m_instruments.begin(); node != m_instruments.end(); )
            {
                list<CInstrument*>::iterator next = node;
                next++;
                CInstrument* instrument = *node;

                instrument->m_reverb.SetProportions(0, 1);
                instrument->m_duration = instrument->m_duration + .025 * 20;
                node = next;

            }
        }
        if (note->Instrument() == L"chorus")
        {
            for (list<CInstrument*>::iterator node = m_instruments.begin(); node != m_instruments.end(); )
            {
                list<CInstrument*>::iterator next = node;
                next++;
                CInstrument* instrument = *node;

                instrument->m_chorus.SetProportions(0, 1);
                node = next;

            }
        }
        if (note->Instrument() == L"compressor")
        {
            for (list<CInstrument*>::iterator node = m_instruments.begin(); node != m_instruments.end(); )
            {
                list<CInstrument*>::iterator next = node;
                next++;
                CInstrument* instrument = *node;

                instrument->m_compressor.SetProportions(.5, .5);
                node = next;

            }
        }
        if (note->Instrument() == L"gate")
        {
            for (list<CInstrument*>::iterator node = m_instruments.begin(); node != m_instruments.end(); )
            {
                list<CInstrument*>::iterator next = node;
                next++;
                CInstrument* instrument = *node;

                instrument->m_noiseGate.SetProportions(.5, .5);
                node = next;

            }
        }

        m_currentNote++;
    }


    //
    // Phase 2: Clear all channels to silence 
    //

    for (int c = 0; c < GetNumChannels(); c++)
    {
        frame[c] = 0;
    }

    //
    // Phase 3: Play an active instruments
    //

    //
    // We have a list of active (playing) instruments.  We iterate over 
    // that list.  For each instrument we call generate, then add the
    // output to our output frame.  If an instrument is done (Generate()
    // returns false), we remove it from the list.
    //

    for (list<CInstrument*>::iterator node = m_instruments.begin(); node != m_instruments.end(); )
    {
        // Since we may be removing an item from the list, we need to know in 
        // advance, what is after it in the list.  We keep that node as "next"
        list<CInstrument*>::iterator next = node;
        next++;

        // Get a pointer to the allocated instrument
        CInstrument* instrument = *node;

        // Call the generate function
        if (instrument->Generate())
        {
            // If we returned true, we have a valid sample.  Add it 
            // to the frame.
            for (int c = 0; c < GetNumChannels(); c++)
            {
                frame[c] += instrument->Frame(c);
            }
        }
        else
        {
            // If we returned false, the instrument is done.  Remove it
            // from the list and delete it from memory.
            m_instruments.erase(node);
            delete instrument;
        }

        // Move to the next instrument in the list
        node = next;
    }
    //
    // Phase 4: Advance the time and beats
    //

    // Time advances by the sample period
    m_time += GetSamplePeriod();

    // Beat advances by the sample period divided by the 
    // number of seconds per beat.  The inverse of seconds
    // per beat is beats per second.
    m_beat += GetSamplePeriod() / m_secperbeat;

    // When the measure is complete, we move to
    // a new measure.  We might be a fraction into
    // the new measure, so we subtract out rather 
    // than just setting to zero.
    if (m_beat > m_beatspermeasure)
    {
        m_beat -= m_beatspermeasure;
        m_measure++;
    }
    //
    // Phase 5: Determine when we are done
    //

    // We are done when there is nothing to play.  We'll put something more 
    // complex here later.
    return !m_instruments.empty() || m_currentNote < (int)m_notes.size();
}

void CSynthesizer::Clear(void)
{
    m_instruments.clear();
    m_notes.clear();

}





void CSynthesizer::XmlLoadInstrument(IXMLDOMNode* xml)
{
    wstring instrument = L"";
    CComBSTR nameTest;
    xml->get_nodeName(&nameTest);

    // Get a list of all attribute nodes and the
    // length of that list

    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    // Loop over the list of attributes
    for (int i = 0; i < len; i++)
    {
        // Get attribute i
        CComPtr<IXMLDOMNode> attrib;
        attributes->get_item(i, &attrib);

        // Get the name of the attribute
        CComBSTR name;
        attrib->get_nodeName(&name);

        // Get the value of the attribute.  
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == "instrument")
        {
            instrument = value.bstrVal;
        }
    }


    CComPtr<IXMLDOMNode> node;
    xml->get_firstChild(&node);
    for (; node != NULL; NextNode(node))
    {
        // Get the name of the node
        CComBSTR name;
        node->get_nodeName(&name);

        if (name == L"note")
        {
            XmlLoadNote(node, instrument);
        } 
    }
    std::sort(std::begin(m_notes), std::end(m_notes),
        [](CNote const& a, CNote const& b) -> bool
        { 
            if (a.Measure() < b.Measure())
            {
                return true;
            }
            if (a.Measure() == b.Measure())
            {
                if (a.Beat() < b.Beat())
                {
                    return true;
                }
            }
            return false;
        });
    
}

void CSynthesizer::XmlLoadNote(IXMLDOMNode* xml, std::wstring& instrument)
{
    m_notes.push_back(CNote());
    m_notes.back().XmlLoad(xml, instrument);
}