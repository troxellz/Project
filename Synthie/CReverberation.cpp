#include "pch.h"
#include "CReverberation.h"
#include <vector>
#include <algorithm>

CReverberation::CReverberation()
{
	m_dry = 1;
	m_wet = 0;
	m_delay = 1103;
	m_wrloc = 0;
	m_rdloc = 0;
	m_samples.resize(m_delay * 100);
	std::fill(m_samples.begin(), m_samples.end(), 0);
	m_decay = .9;
}

void CReverberation::Process(double* frameIn, double* frameOut)
{
	// Instead of doing fancy calculations I just did 100 echos which 
	// Will will fade out for any reasonable decay
	int QSIZE = m_delay * 100 - 1;

	for (int c = 0; c < 2; c++)
	{
		double calcValue = 0;
		bool end = false;
		int x = 0;
		int y = 0;
		double divisor = 0;
		while (!end)
		{
			end = true;
			int test = (m_rdloc - (x) * m_delay + c) % QSIZE;
			if (test < 0)
			{
				test += QSIZE;
			}
			if ( x < 100)
			{
				calcValue += m_samples[test] * pow(m_decay, x);
				end = false;
				divisor += double(pow(m_decay, x));
			}

			if (x % 2 == 0)
			{
				y++;
			}
			x++;

		}
		calcValue = calcValue / (divisor-1);
		frameOut[c] = m_dry * frameIn[c] + m_wet * calcValue;
	}
	m_samples[m_wrloc] = frameIn[0];
	m_samples[m_wrloc + 1] = frameIn[1];

	m_wrloc = (m_wrloc + 2) % QSIZE;
	m_rdloc = (m_rdloc + 2) % QSIZE;
}

void CReverberation::SetProportions(double dry, double wet)
{
	m_dry = dry;
	m_wet = wet;
}

void CReverberation::SetDelay(int delay, int sampleRate)
{
	m_delay = delay * sampleRate / 1000;
}

void CReverberation::SetDecay(double decay)
{
	m_decay = decay;
}

void CReverberation::XmlLoad(IXMLDOMNode* xml)
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
			value.ChangeType(VT_I4);
			SetDelay(value.intVal, 44100);
		}
		else if (name == "decay")
		{
			value.ChangeType(VT_R8);
			m_decay = value.dblVal;
		}

	}

}