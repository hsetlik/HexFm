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
    for(int o = 0; o < numOperators; ++o)
    {
        operators.add(new Operator(o, voiceIndex, tree));
        auto oStr = juce::String(o);
        opAudibleIds[o] = "audibleParam" + oStr;
        for(int i = 0; i < numOperators; ++i)
        {
            auto iStr = juce::String(i);
            opRoutingIds[o][i] = oStr + "to" + iStr + "Param";
        }
    }
    for(int n = 0; n < totalLfos; ++n)
    {
        lfoBank.add(new LfoProcessor(n, tree));
    }
    
}
void FmVoice::updateParams()
{
    for(op1Index = 0; op1Index < TOTAL_OPERATORS; ++op1Index)
    {
        operators[op1Index]->updateParams();
        opAudible[op1Index] = getValue(opAudibleIds[op1Index]);
        if(op1Index < TOTAL_LFOS)
            lfoBank[op1Index]->updateParams();
        for(op2Index = 0; op2Index < TOTAL_OPERATORS; ++op2Index)
        {
            opRouting[op1Index][op2Index] = (int)getValue(opRoutingIds[op1Index][op2Index]);
        }
    }
}
void FmVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
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
                if(opRouting[op1Index][op2Index])
                    { d->modOffset += o->lastOutputSample;}
                ++op2Index;
            }
            opSample = o->sample(fundamental);
            if(opAudible[o->getIndex()])
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
    if(thisLfo->target > 0)
    {
      if(thisLfo->target % 2 != 0)
          operators[(thisLfo->target / 2)]->setAM((1.0f + lfoValue) / 2.0f);
      else
      {
          auto targetOp = thisLfo->target / 2;
          operators[targetOp - 1]->modulateRatio(lfoValue, thisLfo->ratioModType);
      }
    }
}
