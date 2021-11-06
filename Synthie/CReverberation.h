#pragma once
#include <vector>
#include "CSineWave.h"

class CReverberation
{
public:
	//! Constructor
	CReverberation();

	//!Proccesses the incoming sound
	void Process(double* frameIn, double* frameOut);

	//! Sets the portion of wet and dry
	void SetProportions(double dry, double wet);



private:
	//!portion of dry sound
	double m_dry;
	//!portion of wet sound
	double m_wet;
	//! The position in which to read from
	int m_rdloc;
	//! The position in which to write to
	int m_wrloc;

	//! The previous samples to add the other "voices" from
	std::vector<double> m_samples;

	//!how many samples we are delaying by.
	int m_delay;

};

