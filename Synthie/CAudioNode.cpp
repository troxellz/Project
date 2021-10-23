#include "pch.h"
#include "CAudioNode.h"

CAudioNode::CAudioNode(void)
{
    m_frame[0] = 0;
    m_frame[1] = 0;
    m_sampleRate = 44100;
    m_samplePeriod = 1.0 / 44100.0;
    m_bpm = 120.0;
}