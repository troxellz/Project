#include "pch.h"
#include "CPianoEnvelope.h"

CPianoEnvelope::CPianoEnvelope(void)
{
	m_attack = 0.05;
	m_release = 0.05;
	m_bpm = 0;

}

CPianoEnvelope::~CPianoEnvelope(void)
{

}

double CPianoEnvelope::Frame(int c)
{


	if (m_time < m_attack)
	{
		double result = (m_source->Frame(c)) * (m_time / m_attack);
		return (result);
	}
	else if (m_time > ((m_duration / (m_bpm / 60.0)) - m_release))
	{
		return ((m_source->Frame(c)) * (((m_duration / (m_bpm / 60.0)) - m_time) / m_release));
	}
	else
	{
		return (m_source->Frame(c));
	}
}