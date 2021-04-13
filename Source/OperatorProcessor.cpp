/*
  ==============================================================================

    OperatorProcessor.cpp
    Created: 6 Oct 2020 1:04:48pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "OperatorProcessor.h"

float Operator::sample(float fund)
{
    fundamental = fund;
    rawSample = wtOsc.getSample((fund * workingRatio) + (modOffset * modIndex)) * level;
    lastOutputSample = envelope.process(rawSample) * ( 1.0f - amplitudeMod);
    updatePan();
    return lastOutputSample;
}

void Operator::modulateRatio(float value, int mode)
{
    //value is between 0 and 0.6f (just a bit of limiting)
    //value *= 0.6f;
    switch(mode)
    {
        case 0: //upwards only
        {
            maxRatioOffset = baseRatio;
            workingRatio =  baseRatio + (maxRatioOffset * value);
            break;
        }
        case 1: // both directions
        {
            if(value > 0.5f)
            {
                value = (value - 0.5f) * 2.0f;
                maxRatioOffset = baseRatio;
                workingRatio =  baseRatio + (maxRatioOffset * value);
            }
            if(value < 0.5f)
            {
                value *= 2.0f;
                maxRatioOffset = baseRatio / 2.0f;
                workingRatio = baseRatio - (maxRatioOffset * (1.0f - value));
            }
            break;
        }
        case 2: //downwards only
        {
            maxRatioOffset = baseRatio / 2.0f;
            workingRatio = baseRatio - (maxRatioOffset * (1.0f - value));
            break;
        }
    }
}
