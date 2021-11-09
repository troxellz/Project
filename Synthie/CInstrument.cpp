#include "pch.h"
#include "CInstrument.h"
#include "CNoiseGate.h"
#include "CChorusEffect.h"
#include "CCompressor.h"
#include "CReverberation.h"

CInstrument::CInstrument()
{
	m_chorus = CChorusEffect();
	m_reverb = CReverberation();
	m_compressor = CCompressor();
	m_noiseGate = CNoiseGate();

}