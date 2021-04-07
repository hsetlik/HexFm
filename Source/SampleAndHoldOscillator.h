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
        maxOutput = std::numeric_limits<float>::min();
        minOutput = std::numeric_limits<float>::max();
    }
    ~RandomOscillator()
    {
        /*
        printf("Max Output: %f\n", maxOutput);
        printf("Min Output: %f\n", minOutput);
         */
    }
    void hiResTimerCallback() override
    {
        lastRandom = (float) rand() / RAND_MAX;
        lastOutputValue = lastRandom;
    }
    float sample(int msPerCycle)
    {
        if(msPerCycle != lastRate) //only restart the timer if the rate has changed
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
    float maxOutput;
    float minOutput;
    float lastRandom;
    int lastRate = 1000;
    float lastOutputValue = 0.0f;
};

