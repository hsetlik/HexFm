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
    lastOutputSample = envelope.process(osc.sinebuf((fundamental * ratio) + (modOffset * modIndex)) * amplitudeMod);
    return lastOutputSample;
}
