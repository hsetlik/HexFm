/*
  ==============================================================================

    OperatorProcessor.cpp
    Created: 6 Oct 2020 1:04:48pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "OperatorProcessor.h"

float Operator::sample(float fundamental) 
{
    rawSample = wtOsc.getSample((fundamental * ratio) + (modOffset * modIndex));
    lastOutputSample = envelope.process(rawSample) * ( 1.0f - amplitudeMod);
    updatePan();
    return lastOutputSample;
}

void Operator::modulateRatio(float value, int mode)
{
    //value is between 0 and 1
    //min ratio is 0.1f, max is 10.0f
    auto maxIncrease = 10.0f - ratio;
    auto maxDecrease = ratio - 0.1f;
    float modValue;
    switch(mode)
    {
        case 0: //upwards only
        {
            modValue = maxIncrease * value;
            break;
        }
        case 1: // both directions
        {
            auto biValue = (value * 2.0f) - 1.0f; //track value to range -1, 1
            if(biValue > 0.0f)
                modValue = maxIncrease * biValue;
            else
                modValue = maxDecrease * biValue;
            break;
        }
        case 2: //downwards only
        {
            modValue =  -maxDecrease * value;
            break;
        }
    }
    ratio = ratio + modValue;
}
