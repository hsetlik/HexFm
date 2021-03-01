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
    WToscillator(std::vector<float> t) : data(t), position(0.0f), sampleRate(44100.f)
    {
        position = 0.0f;
    }
    ~WToscillator() {}
    void setSampleRate(double rate)
    {
        sampleRate = rate;
    }
    float getSample(double frequency);
    void setTable(std::vector<float> table)
    {
        data = table;
    }
private:
    std::vector<float> data;
    float position;
    float posDelta;
    float output;
    double sampleRate;
    int bottomSampleIndex;
    float sampleDiff;
    float skew;
};

