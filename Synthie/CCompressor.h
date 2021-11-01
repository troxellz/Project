#pragma once
#include <vector>
#include "CSineWave.h"

class CCompressor
{
public:
	//! Constructor
	CCompressor();

	//!Proccesses the incoming sound
	void Process(double* frameIn, double* frameOut);

	//! Sets the portion of wet and dry
	void SetProportions(double dry, double wet);

	//! Set the high and low
	void SetLimits(double high, double low);

	//! Set the amplification and limiting factors
	void SetFactors(double limiter, double amplifier);

private:
	//!portion of dry sound
	double m_dry;
	//!portion of wet sound
	double m_wet;
	//! When to begin to limit sound from the top
	double m_high;
	//! When to start amplifying the low sounds
	double m_low;

	//! The amount to limit by
	double m_limitFactor;

	//! The amount to amplify by
	double m_amplificationFactor;

};
