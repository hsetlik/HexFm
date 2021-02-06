/*
  ==============================================================================

    SampleAndHoldOscillator.h
    Created: 10 Oct 2020 8:56:41am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class RandomOscillator : public juce::HighResolutionTimer
{
public:
    RandomOscillator()
    {
        srand((unsigned) time(NULL));
        startTimer(lastRate);
    }
    ~RandomOscillator() {}
    void hiResTimerCallback() override
    {
        auto randVal = (float) rand() / RAND_MAX;
        lastOutputValue = (randVal - 0.5f) * 2.0f;
    }
    float sample(int msPerCycle)
    {
        if(msPerCycle != lastRate)
        {
            lastRate = msPerCycle;
            stopTimer();
            startTimer(msPerCycle);
            return lastOutputValue;
        }
        else
            return lastOutputValue;
    }
private:
    int lastRate = 1000;
    float lastOutputValue = 0.0f;
};

