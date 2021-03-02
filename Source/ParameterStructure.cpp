/*
  ==============================================================================

    ParameterStructure.cpp
    Created: 15 Feb 2021 12:01:50pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "ParameterStructure.h"

std::vector<float> FmSynthParams::opDelayTime(TOTAL_OPERATORS, 0.0f);
std::vector<float> FmSynthParams::opAttackTime(TOTAL_OPERATORS, 0.0f);
std::vector<float> FmSynthParams::opHoldTime(TOTAL_OPERATORS, 0.0f);
std::vector<float> FmSynthParams::opDecayTime(TOTAL_OPERATORS, 0.0f);
std::vector<float> FmSynthParams::opSustainLevel(TOTAL_OPERATORS, 0.0f);
std::vector<float> FmSynthParams::opReleaseTime(TOTAL_OPERATORS, 0.0f);
std::vector<int> FmSynthParams::opAudible(TOTAL_OPERATORS, 0);

std::vector<float> FmSynthParams::opRatio(TOTAL_OPERATORS, 0.0f);
std::vector<float> FmSynthParams::opLevel(TOTAL_OPERATORS, 0.0f);
std::vector<float> FmSynthParams::opAmplitudeMod(TOTAL_OPERATORS, 0.0f);
std::vector<float> FmSynthParams::opModIndex(TOTAL_OPERATORS, 0.0f);
std::vector<float> FmSynthParams::opEnvLevel(TOTAL_OPERATORS, 0.0f);
std::vector<float> FmSynthParams::opRatioMod(TOTAL_OPERATORS, 0.0f);

std::vector<std::vector<int>> FmSynthParams::opRouting = FmSynthParams::createOpRouting();

std::vector<float> FmSynthParams::lfoRate(TOTAL_LFOS, 0.0f);
std::vector<float> FmSynthParams::lfoLevel(TOTAL_LFOS, 1.0f);
std::vector<int> FmSynthParams::lfoWave(TOTAL_LFOS, 0);
std::vector<int> FmSynthParams::lfoTarget(TOTAL_LFOS, 0);
std::vector<int> FmSynthParams::lfoRatioMode(TOTAL_LFOS, 0);


float FmSynthParams::workingFundamental = 440.0f;

void FmSynthParams::setRouting(juce::AudioProcessorValueTreeState *pTree, std::vector<std::vector<juce::String> > grid)
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
