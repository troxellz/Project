#pragma once
class CNoiseGate
{
public:
	//! Constructor
	CNoiseGate();

	//!Proccesses the incoming sound
	void Process(double* frameIn, double* frameOut);

	//! Sets the portion of wet and dry
	void SetProportions(double dry, double wet);

	//!Set the threshold
	void SetThreshold(double threshold);

private:
	//!portion of dry sound
	double m_dry;
	//!portion of wet sound
	double m_wet;
	//!The level at which to allow sound to begin passing through
	double m_threshold;
};

