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
        std::vector<int> ints;
        for(int n = 0; n < numOperators; ++n)
        {
            int newVal = 0;
            ints.push_back(newVal);
        }
        routingParams.push_back(ints);
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
    for(int i = startSample; i < (startSample + numSamples); ++i)
    {
        for(int lfo = 0; lfo < 4; ++ lfo)
        {
            applyLfo(lfo);
        }
        opSum = 0.0f;
        for(Operator* op : operators)
            {op->cleanOffset();}
        for(Operator* o : operators)
        {
            for(Operator* d : operators)
            {
                if(routingParams[o->getIndex()][d->getIndex()])
                    d->modOffset += o->lastOutputSample;
            }
             opSample = o->sample(fundamental);
            if(o->isAudible)
                opSum += opSample;
        }
        for(int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, i, opSum);
        }
        if(fabs(opSum - lastOpSample) > 0.2f)
            ++numJumps;
        lastOpSample = opSum;
    }
}

void FmVoice::setRoutingFromGrid(juce::AudioProcessorValueTreeState *pTree, std::vector<std::vector<juce::String>> grid)
{
    for(Operator* i : operators)
    {
        for(Operator* n : operators)
        {
            if(*(pTree->getRawParameterValue(grid[i->getIndex()][n->getIndex()])) != 0.0f)
                routingParams[i->getIndex()][n->getIndex()] = 1;
            else
                routingParams[i->getIndex()][n->getIndex()] = 0;
        }
    }
}

void FmVoice::applyModulations()
{
    /*
    for(int source = 0; source < operatorCount; ++source)
    {
        Operator* sourceOp = operators[source];
        sourceOp->cleanOffset();
        for(int dest = 0; dest < operatorCount; ++dest)
        {
            Operator* destOp = operators[dest];
            if(routingParams[source][dest])
            {
                destOp->modOffset += sourceOp->lastOutputSample;
            }
        }
    }
     */
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
