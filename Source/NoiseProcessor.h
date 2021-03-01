/*
  ==============================================================================

    NoiseProcessor.h
    Created: 27 Feb 2021 6:10:12pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class NoiseOscillator
{
public:
    NoiseOscillator()
    {
        randomGen.setSeedRandomly();
    }
    ~NoiseOscillator() {}
    float sample()
    {
        return (randomGen.nextFloat() - 0.5f) * 2.0f; //random value between -1.0 and 1.0
    }
private:
    juce::Random randomGen;
    
};
