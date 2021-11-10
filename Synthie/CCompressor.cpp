#include "pch.h"
#include "CCompressor.h"
#include <vector>
#include <algorithm>

CCompressor::CCompressor()
{
	m_dry = 1;
	m_wet = 0;
	m_low = .01;
	m_high = .05;
	m_amplificationFactor = 2;
	m_limitFactor = 2;
}

void CCompressor::Process(double* frameIn, double* frameOut)
{

	for (int c = 0; c < 2; c++)
	{
		double calcValue = frameIn[c];

		if (abs(frameIn[c]) < m_low)
		{
			calcValue = frameIn[c] * m_amplificationFactor;
			if (calcValue > m_low)
			{
				calcValue = m_low;
			}
			if (calcValue < -m_low)
			{
				calcValue = -m_low;
			}
		}
		if (abs(frameIn[c]) > m_high)
		{
			if (frameIn[c] > 0)
			{
				calcValue = (frameIn[c] - m_high) / m_limitFactor + m_high;

			}
			else
			{
				calcValue = -(m_high + frameIn[c]) / m_limitFactor - m_high;
			}
		}
		frameOut[c] = m_dry * frameIn[c] + m_wet * calcValue;
	}

}

void CCompressor::SetProportions(double dry, double wet)
{
	m_dry = dry;
	m_wet = wet;
}

void CCompressor::SetLimits(double high, double low)
{
	m_high = high;
	m_low = low;
}

void CCompressor::SetFactors(double limiter, double amplifier)
{
	m_limitFactor = limiter;
	m_amplificationFactor = amplifier;
}

void CCompressor::XmlLoad(IXMLDOMNode* xml)
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
		else if (name == "low")
		{
			// Same thing for the beats.
			value.ChangeType(VT_R8);
			m_low = value.dblVal;
		}
		else if (name == "high")
		{
			// Same thing for the beats.
			value.ChangeType(VT_R8);
			m_high = value.dblVal;
		}
		else if (name == "amplification")
		{
			// Same thing for the beats.
			value.ChangeType(VT_R8);
			m_amplificationFactor = value.dblVal;
		}
		else if (name == "limiting")
		{
			// Same thing for the beats.
			value.ChangeType(VT_R8);
			m_limitFactor = value.dblVal;
		}


	}

}