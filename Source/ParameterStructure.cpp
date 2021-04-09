/*
  ==============================================================================

    ParameterStructure.cpp
    Created: 15 Feb 2021 12:01:50pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "ParameterStructure.h"

std::vector<float> ParamStatic::opDelayTime(TOTAL_OPERATORS, 25.0f);
std::vector<float> ParamStatic::opAttackTime(TOTAL_OPERATORS, 25.0f);
std::vector<float> ParamStatic::opHoldTime(TOTAL_OPERATORS, 25.0f);
std::vector<float> ParamStatic::opDecayTime(TOTAL_OPERATORS, 25.0f);
std::vector<float> ParamStatic::opSustainLevel(TOTAL_OPERATORS, 0.5f);
std::vector<float> ParamStatic::opReleaseTime(TOTAL_OPERATORS, 25.0f);
std::vector<int> ParamStatic::opAudible(TOTAL_OPERATORS, 0);

std::vector<float> ParamStatic::opRatio(TOTAL_OPERATORS, 0.0f);
std::vector<float> ParamStatic::opLevel(TOTAL_OPERATORS, 0.0f);
std::vector<float> ParamStatic::opAmplitudeMod(TOTAL_OPERATORS, 0.0f);
std::vector<float> ParamStatic::opModIndex(TOTAL_OPERATORS, 0.0f);
std::vector<float> ParamStatic::opEnvLevel(TOTAL_OPERATORS, 0.0f);
std::vector<float> ParamStatic::opRatioMod(TOTAL_OPERATORS, 0.0f);

std::vector<std::vector<int>> ParamStatic::opRouting = ParamStatic::createOpRouting();

std::vector<float> ParamStatic::lfoRate(TOTAL_LFOS, 0.0f);
std::vector<float> ParamStatic::lfoLevel(TOTAL_LFOS, 1.0f);
std::vector<int> ParamStatic::lfoWave(TOTAL_LFOS, 0);
std::vector<int> ParamStatic::lfoTarget(TOTAL_LFOS, 0);
std::vector<int> ParamStatic::lfoRatioMode(TOTAL_LFOS, 0);

float ParamStatic::workingFundamental = 440.0f;

void ParamStatic::setRouting(juce::AudioProcessorValueTreeState *pTree, std::vector<std::vector<juce::String> > grid)
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
