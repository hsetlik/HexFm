/*
  ==============================================================================

    ParameterStructure.h
    Created: 15 Feb 2021 12:01:50pm
    Author:  Hayden Setlik

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
const int TOTAL_OPERATORS = 6;
const int TOTAL_LFOS = 4;

struct FmSynthParams
{
    static std::vector<float> opDelayTime;
    static std::vector<float> opAttackTime;
    static std::vector<float> opHoldTime;
    static std::vector<float> opDecayTime;
    static std::vector<float> opSustainLevel;
    static std::vector<float> opReleaseTime;
    static std::vector<int> opAudible;
    
    static std::vector<std::vector<int>> createOpRouting()
    {
        std::vector<std::vector<int>> vec;
        for(int i = 0; i < TOTAL_OPERATORS; ++i)
        {
            std::vector<int> newVec(TOTAL_OPERATORS, 0);
            vec.push_back(newVec);
        }
        return vec;
    }
    static std::vector<std::vector<int>> opRouting;
    static std::vector<float> opRatio;
    static std::vector<float> opAmplitudeMod;
    static std::vector<float> opEnvLevel;
    static std::vector<float> opLevel;
    static std::vector<float> opModIndex;
    static std::vector<float> opRatioMod;
    
    static float workingFundamental;
    static std::vector<float> lfoRate;
    static std::vector<float> lfoLevel;
    static std::vector<int> lfoTarget;
    static std::vector<int> lfoWave;
    static std::vector<int> lfoRatioMode;
    static void setRouting(juce::AudioProcessorValueTreeState *pTree, std::vector<std::vector<juce::String>> grid);
};
