/*
  ==============================================================================

    LfoProcessor.cpp
    Created: 9 Oct 2020 9:51:25am
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "LfoProcessor.h"
LfoProcessor::LfoProcessor(int index) : lfoIndex(index)
{
    
}

float LfoProcessor::getSampleValue()
{
    switch(FmSynthParams::lfoWave[lfoIndex])
    {
        case 0:
        {
            lastValue = (lfo_osc.sinebuf(FmSynthParams::lfoRate[lfoIndex]));
            break;
        }
        case 1:
        {
            lastValue = (lfo_osc.triangle(FmSynthParams::lfoRate[lfoIndex]));
            break;
        }
        case 2:
        {
            lastValue = (lfo_osc.square(FmSynthParams::lfoRate[lfoIndex]));
            break;
        }
        case 3:
        {
            lastValue = (lfo_osc.saw(FmSynthParams::lfoRate[lfoIndex]));
            break;
        }
        case 4:
        {
            int msCycle = floor(1000 / (FmSynthParams::lfoRate[lfoIndex] + 0.0001));
            lastValue = randOsc.sample(msCycle);
            break;
        }
    }
    return lastValue * FmSynthParams::lfoLevel[lfoIndex];
};
