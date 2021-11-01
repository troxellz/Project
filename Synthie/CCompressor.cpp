#include "pch.h"
#include "CCompressor.h"
#include <vector>
#include <algorithm>

CCompressor::CCompressor()
{
	m_dry = 1;
	m_wet = 0;
	m_low = .1;
	m_high = .25;
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

void CCompressor::SetProportions(double dry = 1, double wet = 0)
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
