#pragma once
#include "CAudioNode.h"
#include "CNote.h"
#include <vector>
#include "CNoiseGate.h"
#include "CChorusEffect.h"
#include "CCompressor.h"
#include "CReverberation.h"

class CInstrument :
    public CAudioNode
{
private:
     double sends[5];

public:
    CInstrument();

    double m_attack = 0.05;
    double m_release = 0.05;
    double m_duration;
    double m_time;
    virtual void SetNote(CNote* note) = 0;
    void SetDuration(double d) { m_duration = d; }
    double GetDuration() { return m_duration; }
    double Send(int i) { return sends[i]; }
    void SetSends(int i, double val) { sends[i] = val; }


    CNoiseGate m_noiseGate;

    CChorusEffect m_chorus;

    CCompressor m_compressor;

    CReverberation m_reverb;

};

