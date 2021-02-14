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
    auto finalFreq = (fundamental * ratio) + (modOffset * modIndex);
    auto fullAmp = osc.sinebuf(finalFreq);
    lastOutputSample = envelope.process(fullAmp * level * amplitudeMod);
    return lastOutputSample;
}
