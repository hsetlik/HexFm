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
    /*
    if(startSample != 0)
    {
        printf("startSample is non-zero: (%d) in buffer %d\n", startSample, numBuffers);
    }
     */
    for(int i = 0; i < numSamples; ++i)
    {
        //handle FM routing
        //pops not caused by this
        applyModulations();
        //calculate LFO effects
        //also tested OK for popping issue
        for(int lfo = 0; lfo < 4; ++ lfo)
        {
            applyLfo(lfo);
        }
        auto sum = 0.0f;
        //add up the samples from all the operators set to 'audible'
        for(int o = 0; o < operatorCount; ++o)
        {
            auto newSample = operators[o]->sample(fundamental);
            if(operators[o]->isAudible)
                sum += newSample;
        }
        //add that sum to the output buffer
        for(int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, i + startSample, sum);
        }
        auto difference = fabs(sum - lastSample);
        
        if(difference > 0.2)
        {
            ++numJumps;
            printf("Output jumped: %f at sample %d buffer %d\n", difference, i, numBuffers);
        }
        lastSample = sum;
    }
    printf("buffer %d completed\n", numBuffers);
    numBuffers++;
}

void FmVoice::setRoutingFromGrid(juce::AudioProcessorValueTreeState *pTree)
{
    for(int source = 0; source < operatorCount; ++source)
    {
        auto sString = juce::String(source);
        for(int dest = 0; dest < operatorCount; ++dest)
        {
            auto dString = juce::String(dest);
            auto str = sString + "to" + dString + "Param";
            std::atomic<float>* value = pTree->getRawParameterValue(str);
            if(*value)
                routingParams[source][dest] = true;
            else
                routingParams[source][dest] = false;
        }
    }
}

void FmVoice::applyModulations()
{
    for(int i = 0; i < operatorCount; ++i)
    {
        operators[i]->cleanOffset();
    }
    for(int source = 0; source < operatorCount; ++source)
    {
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
