/*
  ==============================================================================

    WavetableProcessor.h
    Created: 1 Mar 2021 12:50:20pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "WavetableData.h"
#define TABLESIZE 512

enum WaveType
{
    Sine,
    Square,
    Saw,
    Tri
};
class OscBase
{ //! abstract base class to provide getSample() and sample rate functionality
public:
    OscBase() : sampleRate(44100.0f), phase(0.0f), phaseDelta(0.0f), lowerIndex(0), upperIndex(0), skew(0.0f)
    {
    }
    virtual ~OscBase() {}
    virtual void setSampleRate(double rate) {sampleRate = rate; }
    virtual float getSample(double frequency) {return 0.0f; }
protected:
    double sampleRate;
    float phase;
    float phaseDelta;
    int lowerIndex;
    int upperIndex;
    float skew;
};
class SineOsc : public OscBase
{ //! basic sine oscillator- no band-limiting
public:
    SineOsc()
    {
        auto dPhase = juce::MathConstants<float>::twoPi / (float)TABLESIZE;
        for(int i = 0; i < TABLESIZE; ++i)
        {
            sineData[i] = std::sin(dPhase * i);
        }
    }
    float getSample(double frequency) override
    {
        phaseDelta = frequency / sampleRate;
        phase += phaseDelta;
        if(phase > 1.0f)
            phase -= 1.0f;
        lowerIndex = (int)floor(phase *  TABLESIZE);
        skew = (phase * TABLESIZE) - lowerIndex;
        upperIndex = (lowerIndex == TABLESIZE - 1) ? 0 : lowerIndex + 1;
        return sineData[lowerIndex] + ((sineData[upperIndex] - sineData[lowerIndex]) * skew);
    }
private:
    std::array<float, TABLESIZE> sineData;
};

class VoiceOscillator
{
public:
    VoiceOscillator() : fData(new float[TABLESIZE]), position(0.0f), sampleRate(44100.f), nyquist(22050.0f)
    {
        auto dX = juce::MathConstants<float>::twoPi / TABLESIZE;
        for(int i = 0; i < TABLESIZE; ++i)
        {
            fData[i] = std::sin(dX * i);
        }
    }
    ~VoiceOscillator() {}
    void setSampleRate(double rate)
    {
        sampleRate = rate;
        nyquist = sampleRate / 2.0f;
    }
    float getSample(double frequency);
private:
    float* fData;
    float position;
    float posDelta;
    float output;
    double sampleRate;
    double nyquist;
    int bottomSampleIndex;
    float sampleDiff;
    float skew;
};

