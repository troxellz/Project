#pragma once
#include <vector>
#include "CSineWave.h"

class CChorusEffect
{
public:
	//! Constructor
	CChorusEffect();

	//!Proccesses the incoming sound
	void Process(double* frameIn, double* frameOut);

	//! Sets the portion of wet and dry
	void SetProportions(double dry, double wet);

	//!Set the number of voices
	void SetVoices(int voices) { m_voices = voices; }

	//! Set the delay
	void SetDelay(int delay, int sampleRate);

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
	//! number of voices
	int m_voices;

	//! The previous samples to add the other "voices" from
	std::vector<double> m_samples;

	//!how many samples we are delaying by.
	int m_delay;

	//! The sinewave which produces variation within the sound
	CSineWave m_wave;
};
