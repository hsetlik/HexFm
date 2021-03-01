/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 28 Feb 2021 9:22:52pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "WavetableOscillator512.h"
float WavetableOscillator512::sample(float freq)
{
            // time for one full period of the note * period of one sample
    delta = (1.0f / freq) * (1.0f / sampleRate);
    position += delta;
    if(position > 1.0f)
    {
        position -= 1.0f;
    }
    bottomIndex = floor(position * 512);
    skew = (position * 512) - bottomIndex;
    upperSample = samples[ceil(position * 512)];
    lowerSample = samples[bottomIndex];
    sampleDiff = upperSample - lowerSample;
    return lowerSample + (skew * sampleDiff);
}
