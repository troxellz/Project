#pragma once
#include "CAudioNode.h"

class CPianoWave :
	public CAudioNode
{
public:
	CPianoWave(void);
	~CPianoWave(void) {};

public:
	virtual void Start();
	virtual bool Generate();

	//! Set the sine wave frequency
	void SetFreq(double f) { m_freq = f; }

	//! Set the sine wave amplitude
	void SetAmplitude(double a) { m_amp = a; }

	//! Get the sine wave amplitude
	double GetAmplitude() { return m_amp; }

private:
	double m_phase;
	double m_amp;
	double m_freq;
};

