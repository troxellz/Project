#include "pch.h"
#include "CNoiseGate.h"

CNoiseGate::CNoiseGate()
{
	m_dry = 0;
	m_wet = 1;
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
