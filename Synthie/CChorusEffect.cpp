#include "pch.h"
#include "CChorusEffect.h"
#include <vector>

CChorusEffect::CChorusEffect()
{
	m_dry = 1;
	m_wet = 0;
	m_delay = 25;
	m_wrloc = m_delay;
	m_rdloc = 0;
	m_wave.SetAmplitude(4);
	m_wave.SetFreq(4);
	m_wave.Start();
}

void CChorusEffect::Process(double* frameIn, double* frameOut)
{
	for (int c = 0; c < 2; c++)
	{
		double calcValue = 0;
		int sampledTimes = 0;
		for (int x = 0; x < 5; x++)
		{
			if (m_samples[m_rdloc + x * m_delay + c] < frameIn[c] + 10 && m_samples[m_rdloc + x * m_delay + c] > frameIn[c] - 10)
			{
				calcValue += m_samples[m_rdloc + x * m_delay + c] + m_wave.Generate();
				sampledTimes++;
			}
		}
		calcValue = calcValue / sampledTimes;
		frameOut[c] = m_dry * frameIn[c] + m_wet * calcValue;
	}
	m_samples[m_wrloc] = frameOut[0];
	m_samples[m_wrloc + 1] = frameOut[1];

	int QSIZE = m_delay * 5;
	m_wrloc = (m_wrloc + 2) % QSIZE;
	m_rdloc = (m_rdloc + 2) % QSIZE;
}

void CChorusEffect::SetProportions(double dry, double wet)
{
	m_dry = dry;
	m_wet = wet;
}

