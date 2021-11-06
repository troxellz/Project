#include "pch.h"
#include "CSequencer.h"
#include "CSynthesizer.h"
#include "xmlhelp.h"
#include <string>
#include <algorithm>

using namespace std;

CSequencer::CSequencer(void)
{
    CoInitialize(NULL);

    m_channels = 2;
    m_sampleRate = 44100.;
    m_samplePeriod = 1 / m_sampleRate;
    m_time = 0;

    m_bpm = 120;
    m_secperbeat = 0.5;
    m_beatspermeasure = 3;
}

void CSequencer::Start(void)
{
    m_measure = 0;
    m_beat = 0;
    m_time = 0;
    for (list<CSynthesizer*>::iterator node = m_snthesizers.begin(); node != m_snthesizers.end(); )
    {
        (*node)->Start();
        node++;
    }
}

void CSequencer::OpenScore(CString& filename)
{
    Clear();

    //
    // Create an XML document
    //

    CComPtr<IXMLDOMDocument>  pXMLDoc;
    bool succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,
        IID_IXMLDOMDocument, (void**)&pXMLDoc));
    if (!succeeded)
    {
        AfxMessageBox(L"Failed to create an XML document to use");
        return;
    }

    // Open the XML document
    VARIANT_BOOL ok;
    succeeded = SUCCEEDED(pXMLDoc->load(CComVariant(filename), &ok));
if (!succeeded || ok == VARIANT_FALSE)
    {
        AfxMessageBox(L"Failed to open XML score file");
        return;
    }

    //
    // Traverse the XML document in memory!!!!
    // Top level tag is <score>
    //

    CComPtr<IXMLDOMNode> node;
    pXMLDoc->get_firstChild(&node);
    for (; node != NULL; NextNode(node))
    {
        // Get the name of the node
        CComBSTR nodeName;
        node->get_nodeName(&nodeName);

        if (nodeName == "score")
        {
            XmlLoadScore(node);
        }
    }

//    sort(m_notes.begin(), m_notes.end());

}

void CSequencer::XmlLoadScore(IXMLDOMNode* xml)
{
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

        // Get the value of the attribute.  A CComVariant is a variable
        // that can have any type. It loads the attribute value as a
        // string (UNICODE), but we can then change it to an integer 
        // (VT_I4) or double (VT_R8) using the ChangeType function 
        // and then read its integer or double value from a member variable.
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == L"bpm")
        {
            value.ChangeType(VT_R8);
            m_bpm = value.dblVal;
            m_secperbeat = 1 / (m_bpm / 60);
        }
        else if (name == L"beatspermeasure")
        {
            value.ChangeType(VT_I4);
            m_beatspermeasure = value.intVal;
        }

    }


    CComPtr<IXMLDOMNode> node;
    xml->get_firstChild(&node);
    for (; node != NULL; NextNode(node))
    {
        // Get the name of the node
        CComBSTR name;
        node->get_nodeName(&name);

        if (name == L"synthesizer")
        {
            XmlLoadSynthesizer(node);
        }
    }
}

void CSequencer::XmlLoadSynthesizer(IXMLDOMNode* xml)
{
    CSynthesizer* synthesizer = NULL;
    synthesizer = new CSynthesizer();
    synthesizer->SetNumChannels(m_channels);
    synthesizer->SetSampleRate(m_sampleRate);
    synthesizer->SetBPM(m_bpm);
    synthesizer->XmlLoadInstrument(xml);
    m_snthesizers.push_back(synthesizer);
}


void CSequencer::Clear(void)
{
    m_snthesizers.clear();
    
}

bool CSequencer::Generate(double* frame)
{
    for (list<CSynthesizer*>::iterator node = m_snthesizers.begin(); node != m_snthesizers.end(); )
    {
        (*node)->Generate(frame);
        node++;
    }
    double audio[2];
    m_chorus.Process(frame, audio);
    frame[0] = audio[0];
    frame[1] = audio[1];
    m_compressor.Process(frame, audio);
    frame[0] = audio[0];
    frame[1] = audio[1];
    m_reverb.Process(frame, audio);
    frame[0] = audio[0];
    frame[1] = audio[1];
    m_noiseGate.Process(frame, frame);
    return true;
}