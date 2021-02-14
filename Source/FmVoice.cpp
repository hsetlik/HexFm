/*
  ==============================================================================

    FmVoice.cpp
    Created: 6 Oct 2020 3:28:58pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "FmVoice.h"

FmVoice::FmVoice(int numOperators, int index) :  voiceIndex(index), operatorCount(numOperators), fundamental(0.0f)
{
    numJumps = 0;
    for(int i = 0; i < numOperators; ++i)
    {
        operators.add(new Operator(i));
    }
    for(int n = 0; n < totalLfos; ++n)
    {
        lfoBank.add(new LfoProcessor(n));
    }
}

float lastSample = 0.0f;
int numBuffers = 0;
void FmVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    
}

void FmVoice::setRoutingFromGrid(juce::AudioProcessorValueTreeState *pTree)
{
    
}

void FmVoice::applyModulations()
{
    for(int source = 0; source < operatorCount; ++source)
    {
        operators[source]->cleanOffset();
        Operator* sourceOp = operators[source];
        for(int dest = 0; dest < operatorCount; ++dest)
        {
            Operator* destOp = operators[dest];
            if(routingParams[source][dest])
            {
                destOp->modOffset += sourceOp->lastOutputSample;
            }
        }
    }
}

void FmVoice::applyLfo(int index)
{
    LfoProcessor* thisLfo = lfoBank[index];
    int target = thisLfo->currentTarget;
    float rawValue = thisLfo->getSampleValue();
    if(target > 0)
    {
        operators[target - 1]->setAmpMod((1.0f + rawValue) / 2.0f);
    }
}
