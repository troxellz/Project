#include "pch.h"
#include "CAR.h"
#include "CSineWave.h"
#include "CAudioNode.h"


CAR::CAR()
{
	m_attack = 0;
	m_release = 0;
	m_duration = 0;
	m_time = 0;
}

void CAR::SetSource(CSineWave source)
{
	m_samplePeriod = source.GetSamplePeriod();
	m_sampleRate = source.GetSampleRate();
}

/*
void CAR::SetSource(CAudioNode source)
{
	m_samplePeriod = source.GetSamplePeriod;
	m_sampleRate = source.GetSampleRate;
}

*/