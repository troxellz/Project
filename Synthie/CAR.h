#pragma once
#include "CSineWave.h"
#include "CAudioNode.h"


class CAR :
    public CAudioNode
{
private:
    double m_attack;
    double m_release;
    double m_duration;
    double m_time;
public:
    CAR();
    ~CAR();

    void SetSource(CSineWave source);
};

