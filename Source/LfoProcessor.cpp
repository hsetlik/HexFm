/*
  ==============================================================================

    LfoProcessor.cpp
    Created: 9 Oct 2020 9:51:25am
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "LfoProcessor.h"
LfoProcessor::LfoProcessor(int index, juce::AudioProcessorValueTreeState* t) : tree(t), lfoIndex(index)
{
    auto iStr = juce::String(index);
    rateId = "lfoRateParam" + iStr;
    levelId = "lfoLevelParam" + iStr;
    targetId = "lfoTargetParam" + iStr;
    ratioModId = "lfoRatioModeParam" + iStr;
    updateParams();
}

float LfoProcessor::getSampleValue()
{
    switch(ParamStatic::lfoWave[lfoIndex])
    {
        case 0:
        {
            lastValue = ((lfo_osc.sinebuf(rate) * 0.5f) + 0.5f);
            break;
        }
        case 1:
        {
            lastValue = ((lfo_osc.triangle(rate) * 0.5f) + 0.5f);
            break;
        }
        case 2:
        {
            lastValue = ((lfo_osc.square(rate) * 0.5f) + 0.5f);
            break;
        }
        case 3:
        {
            lastValue = ((lfo_osc.saw(rate) / 2.0f) + 0.5f);
            break;
        }
        case 4:
        {
            int msCycle = floor(1000 / (rate + 0.0001));
            lastValue = randOsc.sample(msCycle);
            break;
        }
    }
    return lastValue * level;
};

void LfoProcessor::updateParams()
{
    rate = getValue(rateId);
    level = getValue(levelId);
    target = (int)getValue(targetId);
    wave = (int)getValue(waveId);
    ratioModType = (int)getValue(ratioModId);
}
