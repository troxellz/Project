#include "pch.h"
#include "CPianoWave.h"

CPianoWave::CPianoWave(void)
{
	m_phase = 0;
	m_amp = 1;
	m_freq = 440;
}

void CPianoWave::Start()
{
	m_phase = 0;
}

bool CPianoWave::Generate()
{
	m_frame[0] = m_amp * 0.1 * sin(m_phase * 2 * PI);
	m_frame[1] = m_frame[0];

	double test = GetSamplePeriod();
	m_phase += m_freq * test;

	return true;

}