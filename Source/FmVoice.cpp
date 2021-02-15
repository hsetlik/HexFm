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
int op1Index = 0;
int op2Index = 0;
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
        op1Index = 0;
        for(Operator* o : operators)
        {
            op2Index = 0;
            for(Operator* d : operators)
            {
                if(FmSynthParams::opRouting[op1Index][op2Index])
                    d->modOffset += o->lastOutputSample;
                ++op2Index;
            }
            opSample = o->sample(fundamental);
            if(FmSynthParams::opAudible[o->getIndex()])
                opSum += opSample;
            ++op1Index;
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
void FmVoice::applyLfo(int index)
{
    LfoProcessor* thisLfo = lfoBank[index];
    lfoValue = thisLfo->getSampleValue();
    if(FmSynthParams::lfoTarget[index] > 0)
    {
        FmSynthParams::opAmplitudeMod[FmSynthParams::lfoTarget[index] - 1] = ((1.0f + lfoValue) / 2.0f);
    }
}
