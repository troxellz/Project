#include "pch.h"
#include "CChorusEffect.h"
#include <vector>
#include <algorithm>

CChorusEffect::CChorusEffect()
{
	m_dry = 1;
	m_wet = 0;
	m_voices = 5;
	m_delay = 2206;
	m_wrloc = 0;
	m_rdloc = 0;
	m_wave.SetAmplitude(1);
	m_wave.SetFreq(3);
	m_wave.Start();
	m_samples.resize(m_delay * m_voices);
	std::fill(m_samples.begin(), m_samples.end(), 0);
}

void CChorusEffect::Process(double* frameIn, double* frameOut)
{
	int QSIZE = m_delay * m_voices - 1;

	for (int c = 0; c < 2; c++)
	{
		double calcValue = 0;
		int sampledTimes = 0;
		for (int x = 0; x < m_voices; x++)
		{
			int test = (m_rdloc - x * m_delay + c) % QSIZE;
			if (test < 0)
			{
				test += QSIZE;
			}
			if (m_samples[test] < frameIn[c] + 100 && m_samples[test] > frameIn[c] - 100)
			{
				calcValue += m_samples[test] + m_wave.Generate() * .01;
				sampledTimes++;
			}
		}
		calcValue = calcValue / sampledTimes;
		frameOut[c] = m_dry * frameIn[c] + m_wet * calcValue;
	}
	m_samples[m_wrloc] = frameIn[0];
	m_samples[m_wrloc + 1] = frameIn[1];

	m_wrloc = (m_wrloc + 2) % QSIZE;
	m_rdloc = (m_rdloc + 2) % QSIZE;
}

void CChorusEffect::SetProportions(double dry, double wet)
{
	m_dry = dry;
	m_wet = wet;
}

void CChorusEffect::SetDelay(int delay, int sampleRate)
{
	m_delay = delay * sampleRate / 1000;
}

void CChorusEffect::XmlLoad(IXMLDOMNode* xml)
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

		// Get the value of the attribute.  
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "dry")
		{
			// The file has measures that start at 1.  
			// We'll make them start at zero instead.
			//value.ChangeType(VT_I4);
			value.ChangeType(VT_R8);
			SetProportions(value.dblVal, 1 - value.dblVal);
		}
		else if (name == "delay")
		{
			// Same thing for the beats.
			value.ChangeType(VT_I4);
			SetDelay(value.intVal, 44100);
		}
		else if (name == "voices")
		{
			// Same thing for the beats.
			value.ChangeType(VT_I4);
			m_voices = value.intVal;
		}

	}

}