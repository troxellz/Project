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

	//! Set the delay in milliseconds assuming 44100 sample rate
	void SetDelay(int delay, int sampleRate);

	void SetDecay(double decay);

	//!Load
	void XmlLoad(IXMLDOMNode* xml);


private:
	//!portion of dry sound
	double m_dry;
	//!portion of wet sound
	double m_wet;
	//! The position in which to read from
	int m_rdloc;
	//! The position in which to write to
	int m_wrloc;
	//! The decay factor for each echo
	double m_decay;

	//! The previous samples to add the other "voices" from
	std::vector<double> m_samples;

	//!how many samples we are delaying by.
	int m_delay;

};

