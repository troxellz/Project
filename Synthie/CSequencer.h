#pragma once
#include <list>
#include "CInstrument.h"
#include "CSynthesizer.h"
#include "CNoiseGate.h"
#include "CChorusEffect.h"
#include "CCompressor.h"
#include "CReverberation.h"
#include "msxml2.h"
#include <vector>
#include "CNote.h"

class CSequencer
{

public:
    //! Constructor
    CSequencer();

    //! Number of audio channels
    int GetNumChannels() { return m_channels; }

    //! Sample rate in samples per second
    double GetSampleRate() { return m_sampleRate; }

    //! Sample period in seconds (1/samplerate)
    double GetSamplePeriod() { return m_samplePeriod; }

    //! Set the number of channels
    void SetNumChannels(int n) { m_channels = n; }

    //! Set the sample rate
    void SetSampleRate(double s) { m_sampleRate = s;  m_samplePeriod = 1.0 / s; }

    //! Start the synthesizer
    void Start(void);


    //! Generate one audio frame
    bool Generate(double* frame);

    void Clear(void);

    void OpenScore(CString& filename);


    //! Get the time since we started generating audio
    double GetTime() { return m_time; }

private:
    int		m_channels;
    double	m_sampleRate;
    double	m_samplePeriod;
    double m_time;
    std::list<CSynthesizer*> m_snthesizers;
    std::list<CInstrument*>  m_instruments;
    double  m_bpm;                  //!< Beats per minute
    int     m_beatspermeasure;  //!< Beats per measure
    double  m_secperbeat;        //!< Seconds per beat
    std::vector<CNote> m_notes;
    int m_currentNote;          //!< The current note we are playing
    int m_measure;              //!< The current measure
    double m_beat;              //!< The current beat within the measure

    void XmlLoadScore(IXMLDOMNode* xml);
    void XmlLoadSynthesizer(IXMLDOMNode* xml);
    CNoiseGate m_noiseGate;

    CChorusEffect m_chorus;

    CCompressor m_compressor;

    CReverberation m_reverb;
};

