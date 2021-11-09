#pragma once
#include "CInstrument.h"
#include "CPianoWave.h"
#include "CPianoEnvelope.h"

#include <vector>


class CPianoInstrument :
    public CInstrument
{
public:
	CPianoInstrument();
	virtual ~CPianoInstrument();

public:
	virtual void Start();
	virtual bool Generate();
	virtual void SetNote(CNote* note);



	void SetPedal(bool pedal) { m_pedal = pedal; }
	void SetVolume(double vol) { m_volume = vol; }
	void SetDuration(double d) { m_duration = d; }
	void SetFreq(double f) { m_pianowave.SetFreq(f); }



private:
	CPianoWave m_pianowave;

	double m_duration;
	double m_time;
	double m_attack;
	double m_release;
	double m_volume;
	bool m_pedal;

	CPianoEnvelope m_ar;
};

