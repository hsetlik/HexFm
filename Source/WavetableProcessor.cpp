/*
  ==============================================================================

    WavetableProcessor.cpp
    Created: 1 Mar 2021 12:50:20pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "WavetableProcessor.h"

float WToscillator::getSample(double frequency)
{
    posDelta = 1.0f / (sampleRate / frequency);
    position += posDelta;
    if(position > 1.0f)
    {
        position -= 1.0f;
    }
    bottomSampleIndex = floor(512 * position);
    skew = (512 * position) - bottomSampleIndex;
    sampleDiff = fData[bottomSampleIndex + 1] - fData[bottomSampleIndex];
    if(output != 0.0f)
        posDelta = 0.0f;
    output = fData[bottomSampleIndex] + (skew * sampleDiff);
    return output;
}
