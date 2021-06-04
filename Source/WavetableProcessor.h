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
#include "FFT.h"

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
    void setSampleRate(double rate)override
    {
        sampleRate = rate;
    }
    float getSample(double frequency) override
    {
        phaseDelta = (float)(frequency / sampleRate);
        phase += phaseDelta;
        if(phase > 1.0f)
            phase -= 1.0f;
        lowerIndex = floor(phase *  TABLESIZE);
        skew = (phase * TABLESIZE) - lowerIndex;
        upperIndex = (lowerIndex == TABLESIZE - 1) ? 0 : lowerIndex + 1;
        output = sineData[lowerIndex] + ((sineData[upperIndex] - sineData[lowerIndex]) * skew);
        return output;
    }
private:
    float output;
    float sineData[TABLESIZE];
};
//! simple struct to hold a wavetable
struct Wavetable
{
    float table[TABLESIZE];
    float minFreq;
    float maxFreq;
};

class WavetableFrame
{//! WavetableFrame class from Octane works by itself as a simple oscillator
public:
    WavetableFrame(WaveType type);
    float getSample(double hz);
    void setSampleRate(double rate)
    {
        sampleRate = rate;
    }
    void createTables(int size, float* real, float* imag);
    float makeTable(float* waveReal, float* waveImag, int numSamples, float scale, float bottomFreq, float topFreq);
    Wavetable* tableForHz(double hz);
private:
    juce::OwnedArray<Wavetable> tables;
    float phase;
    float phaseDelta;
    double sampleRate;
    int tablesAdded;
    int bottomIndex;
    float bSample;
    float tSample;
    float skew;
};

class BandLimitedOsc : public OscBase
{
public:
    BandLimitedOsc(WaveType type);
    float getSample(double frequency) override
    {
        return frame.getSample(frequency);
    }
    void setSampleRate(double rate) override
    {
        sampleRate = rate;
        frame.setSampleRate(rate);
    }
private:
    WavetableFrame frame;
};


class VoiceOscillator
{
public:
    VoiceOscillator() :
    currentWaveType(Sine),
    pOsc(std::make_unique<SineOsc>())
    {
        
    }
    ~VoiceOscillator() {}
    void setSampleRate(double rate)
    {
        pOsc->setSampleRate(rate);
    }
    float getSample(double frequency)
    {
        return pOsc->getSample(frequency);
    }
private:
    WaveType currentWaveType;
    std::unique_ptr<OscBase> pOsc;
};

