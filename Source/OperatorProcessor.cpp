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
    rawSample = osc.sinebuf((fundamental * FmSynthParams::opRatio[index]) + (modOffset * FmSynthParams::opModIndex[index])); //* FmSynthParams::opAmplitudeMod[index];
    lastOutputSample = envelope.process(rawSample);
    return lastOutputSample;
}
