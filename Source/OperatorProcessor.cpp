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
    rawSample = wtOsc.getSample((fundamental * (ParamStatic::opRatio[index] + ParamStatic::opRatioMod[index])) + (modOffset * ParamStatic::opModIndex[index]));
    lastOutputSample = envelope.process(rawSample) * ( 1.0f - ParamStatic::opAmplitudeMod[index]);
    return lastOutputSample;
}
