#pragma once
#include "CAudioNode.h"
#include "CNote.h"
class CInstrument :
    public CAudioNode
{
private:

public:
    double m_attack = 0.05;
    double m_release = 0.05;
    double m_duration;
    double m_time;
    virtual void SetNote(CNote* note) = 0;
    void SetDuration(double d) { m_duration = d; }
    double GetDuration() { return m_duration; }

};

