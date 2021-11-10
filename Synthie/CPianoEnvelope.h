#pragma once
#include "CAudioNode.h"
#include "CPianoWave.h"


class CPianoEnvelope :
    public CAudioNode
{
public:
    CPianoEnvelope();
    virtual ~CPianoEnvelope();

    virtual void Start() override {};
    virtual bool Generate() override { m_source->Generate(); return true; };

    void SetSource(CPianoWave* source) { m_source = source; };
    void SetSampleRate(double sampleRate) { m_sampleRate = sampleRate; };
    void SetDuration(double duration) { m_duration = duration; };
    void SetTime(double time) { m_time = time; };
    void SetBPM(double bpm) { m_bpm = bpm; };
    void SetRelease(double release) { m_release = release; };

    //! Access one channel of a generated audio frame
    double Frame(int c);

private:
    double m_attack;
    double m_release;
    double m_duration;
    double m_time;
    double m_sampleRate;
    double m_bpm;

    CPianoWave* m_source;
};

