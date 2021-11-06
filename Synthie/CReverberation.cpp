#include "pch.h"
#include "CReverberation.h"
#include <vector>
#include <algorithm>

CReverberation::CReverberation()
{
	m_dry = .5;
	m_wet = .5;
	m_delay = 1103;
	m_wrloc = 0;
	m_rdloc = 0;
	m_samples.resize(m_delay * 100);
	std::fill(m_samples.begin(), m_samples.end(), 0);
}

void CReverberation::Process(double* frameIn, double* frameOut)
{
	int QSIZE = m_delay * 50 - 1;
	double decay = .9;

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
			if ( x < 50)
			{
				calcValue += m_samples[test] * pow(decay, x);
				end = false;
				divisor += double(pow(decay, x));
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

