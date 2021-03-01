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

    rawSample = sineOsc.sample((fundamental * FmSynthParams::opRatio[index]) + (modOffset * FmSynthParams::opModIndex[index]));
    lastOutputSample = envelope.process(rawSample) * ( 1.0f - FmSynthParams::opAmplitudeMod[index]);
    return lastOutputSample;
}
