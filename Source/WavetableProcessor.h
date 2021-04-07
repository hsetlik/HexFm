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

class WToscillator
{
public:
    WToscillator(std::vector<float> t) : position(0.0f), sampleRate(44100.f)
    {
        auto length = (int)t.size();
        fData = new float[length];
        for(int i = 0; i < length; ++i)
        {
            fData[i] = t[i];
        }
    }
    ~WToscillator() {}
    void setSampleRate(double rate)
    {
        sampleRate = rate;
    }
    float getSample(double frequency);
private:
    float* fData;
    float position;
    float posDelta;
    float output;
    double sampleRate;
    int bottomSampleIndex;
    float sampleDiff;
    float skew;
};

