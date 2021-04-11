/*
  ==============================================================================

    ParameterStructure.cpp
    Created: 15 Feb 2021 12:01:50pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "ParameterStructure.h"

bool ParamStatic::routingHasChanged = false;

AtomicParam<float> ParamStatic::opDelayTime[TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opAttackTime[TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opHoldTime[TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opDecayTime[TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opSustainLevel[TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opReleaseTime[TOTAL_OPERATORS];

AtomicParam<int> ParamStatic::opAudible[TOTAL_OPERATORS];

AtomicParam<int> ParamStatic::opRouting[TOTAL_OPERATORS][TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opRatio[TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opAmplitudeMod[TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opEnvLevel[TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opLevel[TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opModIndex[TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opRatioMod[TOTAL_OPERATORS];
AtomicParam<float> ParamStatic::opPanValue[TOTAL_OPERATORS];

AtomicParam<float> ParamStatic::lfoRate[TOTAL_LFOS];
AtomicParam<float> ParamStatic::lfoLevel[TOTAL_LFOS];
AtomicParam<int> ParamStatic::lfoTarget[TOTAL_LFOS];
AtomicParam<int> ParamStatic::lfoWave[TOTAL_LFOS];
AtomicParam<int> ParamStatic::lfoRatioMode[TOTAL_LFOS];

float ParamStatic::workingFundamental = 440.0f;

void ParamStatic::setRouting(juce::AudioProcessorValueTreeState *pTree, std::vector<std::vector<juce::String>> grid)
{
    for(int i = 0; i < TOTAL_OPERATORS; ++i)
    {
        for(int n = 0; n < TOTAL_OPERATORS; ++n)
        {
            if(*(pTree->getRawParameterValue(grid[i][n])) != 0.0f)
                opRouting[i][n] = 1;
            else
                opRouting[i][n] = 0;
        }
    }
}
