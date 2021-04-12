/*
  ==============================================================================

    ParameterStructure.h
    Created: 15 Feb 2021 12:01:50pm
    Author:  Hayden Setlik

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#define TOTAL_OPERATORS 6
#define TOTAL_LFOS 4

template <typename T>
class AtomicParam
{
public:
    void set(T _value)
    {
        value = _value;
    }
    void setFrom(std::atomic<T>* source)
    {
        value = source->load();
    }
    T get()
    {
        return value.load();
    }
    AtomicParam()
    {
        value = (T)0;
    }
    AtomicParam(T val)
    {
        value = val;
    }
    void operator =(T val)
    {
        value = val;
    }
    bool operator  ==(T val)
    {
        if(val == value.load())
            return true;
        return false;
    }
    bool operator >(T val)
    {
        return value.load() > val;
    }
private:
    std::atomic<T> value;
};

//hold all the parameters for a given patch in a single instance
struct ParamStatic
{
    static AtomicParam<float> opDelayTime[TOTAL_OPERATORS];
    static AtomicParam<float> opAttackTime[TOTAL_OPERATORS];
    static AtomicParam<float> opHoldTime[TOTAL_OPERATORS];
    static AtomicParam<float> opDecayTime[TOTAL_OPERATORS];
    static AtomicParam<float> opSustainLevel[TOTAL_OPERATORS];
    static AtomicParam<float> opReleaseTime[TOTAL_OPERATORS];
   
    static AtomicParam<int> opAudible[TOTAL_OPERATORS];;
    
    static AtomicParam<int> opRouting[TOTAL_OPERATORS][TOTAL_OPERATORS];
    static AtomicParam<float> opRatio[TOTAL_OPERATORS];
    static AtomicParam<float> opAmplitudeMod[TOTAL_OPERATORS];
    static AtomicParam<float> opEnvLevel[TOTAL_OPERATORS];
    static AtomicParam<float> opLevel[TOTAL_OPERATORS];
    static AtomicParam<float> opModIndex[TOTAL_OPERATORS];
    static AtomicParam<float> opRatioMod[TOTAL_OPERATORS];
    static AtomicParam<float> opPanValue[TOTAL_OPERATORS];
    static AtomicParam<int> routingHasChanged;
    
    static float workingFundamental;
    static AtomicParam<float> lfoRate[TOTAL_LFOS];
    static AtomicParam<float> lfoLevel[TOTAL_LFOS];
    static AtomicParam<int> lfoTarget[TOTAL_LFOS];
    static AtomicParam<int> lfoWave[TOTAL_LFOS];
    static AtomicParam<int> lfoRatioMode[TOTAL_LFOS];
    static void setRouting(juce::AudioProcessorValueTreeState *pTree, std::vector<std::vector<juce::String>> grid);
};
