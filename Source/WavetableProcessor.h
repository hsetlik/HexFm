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

class SineTableOscillator
{
public:
    SineTableOscillator() : fData(new float[TABLESIZE]), position(0.0f), sampleRate(44100.f), nyquist(22050.0f)
    {
        auto dX = juce::MathConstants<float>::twoPi / TABLESIZE;
        for(int i = 0; i < TABLESIZE; ++i)
        {
            fData[i] = std::sin(dX * i);
        }
    }
    ~SineTableOscillator() {}
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

