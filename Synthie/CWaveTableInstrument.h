#pragma once
#include<vector>
#include<list>
#include<string>
using namespace std;


#include "CInstrument.h"
class CWaveTableInstrument :
    public CInstrument
{
    

    private:
        int numSampleFrames;
        CSineWave  m_sinewave;
        double m_time;
        int numChannels;
        int sampleRate;
        int pos;


    public:
        CWaveTableInstrument();

        virtual void Start();
        virtual bool Generate();

        void SetFreq(double f) { m_sinewave.SetFreq(f); }
        void SetAmplitude(double a) { m_sinewave.SetAmplitude(a); }
        void SetNote(CNote * note);
        //void GetTime() { return m_time; }
        void LoadSample(const wchar_t *filename);
        //! vector to get wavetable
        vector<short> m_wavetable;
        
        int GetNumChannels() { return numChannels; }
        int SampleFrames() {
            return numSampleFrames;
        }


    //private:
        


    };



