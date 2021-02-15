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
    switch(currentWaveType)
    {
        case 0:
        {
            lastValue = (lfo_osc.sinebuf(currentRate));
            break;
        }
        case 1:
        {
            lastValue = (lfo_osc.triangle(currentRate));
            break;
        }
        case 2:
        {
            lastValue = (lfo_osc.square(currentRate));
            break;
        }
        case 3:
        {
            lastValue = (lfo_osc.saw(currentRate));
            break;
        }
        case 4:
        {
            int msCycle = floor(1000 / (currentRate + 0.0001));
            lastValue = randOsc.sample(msCycle);
            break;
        }
    }
    return lastValue * currentLevel;
};
