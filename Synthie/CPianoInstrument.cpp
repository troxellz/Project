#include "pch.h"
#include "CPianoInstrument.h"
#include "Notes.h"

using namespace std;

CPianoInstrument::CPianoInstrument()
{
    m_attack = 0.05;
    m_release = 0.25;
    m_volume = 1.0;
    m_duration = 1.0;
    m_pedal = false;
}


CPianoInstrument::~CPianoInstrument()
{
}



void CPianoInstrument::Start()
{
    m_pianowave.SetSampleRate(GetSampleRate());
    m_pianowave.SetAmplitude(m_volume);
    m_pianowave.Start();
    m_time = 0;

    // Tell the AR object it gets its samples from 
    // the sine wave object.
    m_ar.SetSource(&m_pianowave);
    m_ar.SetSampleRate(GetSampleRate());

    if (m_pedal)
    {
        m_duration = 4.0;
        m_ar.SetRelease(3);
    }

    m_ar.SetDuration(m_duration);
    m_ar.SetBPM(GetBPM());
    m_ar.Start();
}


bool CPianoInstrument::Generate()
{
    m_pianowave.Generate();
    m_ar.SetTime(m_time);

    m_frame[0] = m_ar.Frame(0);
    m_frame[1] = m_ar.Frame(1);

    double audio[2];
    m_chorus.Process(m_frame, audio);
    m_frame[0] = audio[0];
    m_frame[1] = audio[1];
    m_compressor.Process(m_frame, audio);
    m_frame[0] = audio[0];
    m_frame[1] = audio[1];
    m_reverb.Process(m_frame, audio);
    m_frame[0] = audio[0];
    m_frame[1] = audio[1];
    m_noiseGate.Process(m_frame, m_frame);

    m_time += GetSamplePeriod();
    
    return m_time < m_duration / (GetBPM() / 60.0);
}

void CPianoInstrument::SetNote(CNote* note)
{
    // Get a list of all attribute nodes and the
    // length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    note->Node()->get_attributes(&attributes);
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

        // Get the value of the attribute.  A CComVariant is a variable
        // that can have any type. It loads the attribute value as a
        // string (UNICODE), but we can then change it to an integer 
        // (VT_I4) or double (VT_R8) using the ChangeType function 
        // and then read its integer or double value from a member variable.
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == "duration")
        {
            value.ChangeType(VT_R8);
            SetDuration(value.dblVal);
        }
        else if (name == "note")
        {
            SetFreq(NoteToFrequency(value.bstrVal));
        }
        else if (name == "pedal")
        {
            wstring pedal_str(value.bstrVal, SysStringLen(value.bstrVal));

            if (pedal_str == L"down")
            {
                m_pedal = true;
            }

        }
        else if (name == "volume")
        {
            value.ChangeType(VT_R8);
            SetVolume(value.dblVal);
        }

    }


}