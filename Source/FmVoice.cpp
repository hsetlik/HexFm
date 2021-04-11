/*
  ==============================================================================

    FmVoice.cpp
    Created: 6 Oct 2020 3:28:58pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "FmVoice.h"



FmVoice::FmVoice(int numOperators, int index, juce::AudioProcessorValueTreeState* t) : tree(t), voiceIndex(index), operatorCount(numOperators), fundamental(1.0f)
{
    lfoMax = std::numeric_limits<float>::min();
    lfoMin = std::numeric_limits<float>::max();
    numJumps = 0;
    for(int i = 0; i < numOperators; ++i)
    {
        operators.add(new Operator(i, voiceIndex, tree));
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
    ParamStatic::workingFundamental = fundamental;
    for(int i = startSample; i < (startSample + numSamples); ++i)
    {
        for(int lfo = 0; lfo < 4; ++ lfo)
        {
            applyLfo(lfo);
        }
        opSum = 0.0f;
        sumL = 0.0f;
        sumR = 0.0f;
        for(Operator* op : operators)
            {op->cleanFreqOffset();}
        op1Index = 0;
        for(Operator* o : operators)
        {
            op2Index = 0;
            for(Operator* d : operators)
            {
                if(ParamStatic::opRouting[op1Index][op2Index])
                    { d->modOffset += o->lastOutputSample;}
                ++op2Index;
            }
            opSample = o->sample(fundamental);
            if(ParamStatic::opAudible[o->getIndex()])
            {
                opSum += opSample;
                sumL += o->lastOutputL;
                sumR += o->lastOutputR;
            }
                
            ++op1Index;
        }
        outputBuffer.addSample(0, i, sumL);
        outputBuffer.addSample(1, i, sumR);
        
        if(fabs(opSum - lastOpSample) > 0.2f)
            ++numJumps;
        lastOpSample = opSum;
    }
}
void FmVoice::applyLfo(int index)
{
    LfoProcessor* thisLfo = lfoBank[index];
    lfoValue = thisLfo->getSampleValue();
    /*
    if(lfoValue < lfoMin)
        lfoMin = lfoValue;
    if(lfoValue > lfoMax)
        lfoMax = lfoValue;
     */
    if(ParamStatic::lfoTarget[index] > 0)
    {
      if(ParamStatic::lfoTarget[index] % 2 != 0)
          ParamStatic::opAmplitudeMod[(ParamStatic::lfoTarget[index] / 2)] = ((1.0f + lfoValue) / 2.0f);
      else
      {
          auto targetOp = ParamStatic::lfoTarget[index] / 2;
          operators[targetOp - 1]->modulateRatio(lfoValue, ParamStatic::lfoRatioMode[index]);
      }
    }
}
