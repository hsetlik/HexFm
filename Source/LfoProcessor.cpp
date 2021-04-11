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
    switch(ParamStatic::lfoWave[lfoIndex].get())
    {
        case 0:
        {
            lastValue = ((lfo_osc.sinebuf(ParamStatic::lfoRate[lfoIndex].get()) * 0.5f) + 0.5f);
            break;
        }
        case 1:
        {
            lastValue = ((lfo_osc.triangle(ParamStatic::lfoRate[lfoIndex].get()) * 0.5f) + 0.5f);
            break;
        }
        case 2:
        {
            lastValue = ((lfo_osc.square(ParamStatic::lfoRate[lfoIndex].get()) * 0.5f) + 0.5f);
            break;
        }
        case 3:
        {
            lastValue = ((lfo_osc.saw(ParamStatic::lfoRate[lfoIndex].get()) / 2.0f) + 0.5f);
            break;
        }
        case 4:
        {
            int msCycle = floor(1000 / (ParamStatic::lfoRate[lfoIndex].get() + 0.0001));
            lastValue = randOsc.sample(msCycle);
            break;
        }
    }
    return lastValue * ParamStatic::lfoLevel[lfoIndex].get();
};
