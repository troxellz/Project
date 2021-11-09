#include "pch.h"
#include "CToneInstrument.h"
#include "CSineWave.h"
#include "CWaveTableInstrument.h"
#include "CNote.h"
#include "Notes.h"
#include <audio/DirSoundSource.h>
#include <string>
#include <algorithm>


CWaveTableInstrument::CWaveTableInstrument(void)
{
    SetDuration(0.1);
    m_time = 0;
    numChannels = 2;
    pos = 0;
    LoadSample(L"filename");
}


void CWaveTableInstrument::Start()
{

    m_sinewave.Start();
    m_time = 0;

}

void CWaveTableInstrument::LoadSample(const wchar_t *filename)
{
    CDirSoundSource wavin;
    if (wavin.Open(filename)) {
        numChannels = wavin.NumChannels();
        sampleRate = wavin.SampleRate();
        numSampleFrames = wavin.NumSampleFrames();

        m_wavetable.resize(numSampleFrames * numChannels);
        for (int i = 0; i < numSampleFrames; i++) {
            short frame[2];
            wavin.ReadFrame(frame);
            m_wavetable[i] = frame[0];

        }

    }
}


bool CWaveTableInstrument::Generate()
{
    // Tell the component to generate an audio sample
    m_sinewave.Generate();


    m_frame[0] = m_wavetable[pos];
    m_frame[1] = m_wavetable[pos];
    pos = (pos + 1) % numSampleFrames;


    
    m_time += GetSamplePeriod();
    // We return true until the time reaches the duration.
    return m_time < GetDuration();
    
}

void CWaveTableInstrument::SetNote(CNote* note)
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
            SetDuration(value.dblVal * (1 / (m_bpm / 60)));
        }
        else if (name == "note")
        {
            SetFreq(NoteToFrequency(value.bstrVal));
        }

    }

}


