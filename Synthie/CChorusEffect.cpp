#include "pch.h"
#include "CChorusEffect.h"
#include <vector>
#include <algorithm>

CChorusEffect::CChorusEffect()
{
	m_dry = .5;
	m_wet = .5;
	m_delay = 2206;
	m_wrloc = m_delay;
	m_rdloc = 0;
	m_wave.SetAmplitude(1);
	m_wave.SetFreq(3);
	m_wave.Start();
	m_samples.resize(m_delay * 5);
	std::fill(m_samples.begin(), m_samples.end(), 0);
}

void CChorusEffect::Process(double* frameIn, double* frameOut)
{
	int QSIZE = m_delay * 5 - 1;

	for (int c = 0; c < 2; c++)
	{
		double calcValue = 0;
		int sampledTimes = 0;
		for (int x = 0; x < 5; x++)
		{
			if (m_samples[(m_rdloc + x * m_delay + c) % QSIZE] < frameIn[c] + 100 && m_samples[(m_rdloc + x * m_delay + c)%QSIZE] > frameIn[c] - 100)
			{
				calcValue += m_samples[(m_rdloc + x * m_delay + c) % QSIZE] + m_wave.Generate() * .001;
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

