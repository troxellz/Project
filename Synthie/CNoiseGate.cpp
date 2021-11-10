#include "pch.h"
#include "CNoiseGate.h"

CNoiseGate::CNoiseGate()
{
	m_dry = 1;
	m_wet = 0;
	m_threshold = .005;
}

void CNoiseGate::Process(double *frameIn, double *frameOut)
{
	for (int c = 0; c < 2; c++)
	{
		double calcValue = 0;

		if (abs(frameIn[c]) > m_threshold)
		{
			calcValue = frameIn[c];
		}
		frameOut[c] = m_dry * frameIn[c] + m_wet * calcValue;
	}
}

void CNoiseGate::SetProportions(double dry, double wet)
{
	m_dry = dry;
	m_wet = wet;
}

void CNoiseGate::SetThreshold(double threshold)
{
	m_threshold = threshold;
}

void CNoiseGate::XmlLoad(IXMLDOMNode* xml)
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
		else if (name == "threshold")
		{
			// Same thing for the beats.
			value.ChangeType(VT_R8);
			m_threshold = value.dblVal;
		}
	}
}