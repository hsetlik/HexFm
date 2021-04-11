/*
  ==============================================================================

    DAHDSR.h
    Created: 6 Oct 2020 1:07:11pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <math.h>
#include "ParameterStructure.h"

class DAHDSR
{
public:
    enum envPhase
    {
        delayPhase,
        attackPhase,
        holdPhase,
        decayPhase,
        sustainPhase,
        releasePhase,
        noteOff
    };
    juce::AudioProcessorValueTreeState* tree;
    //functions
    DAHDSR(int ind, juce::AudioProcessorValueTreeState* t) : tree(t), factor(1.0f), sampleRate(44100), index(ind)
    {
        trigger = false;
        samplesIntoPhase = 0;
        currentPhase = noteOff;
        auto iStr = juce::String(ind);
        delayId = "delayParam" + iStr;
        attackId = "attackParam" + iStr;
        holdId = "holdParam" + iStr;
        decayId = "decayParam" + iStr;
        sustainId = "sustainParam" + iStr;
        releaseId = "releaseParam" + iStr;
    }
    ~DAHDSR() {}
    void triggerOn()
    {
        trigger = true;
        samplesInPhase = floor(ParamStatic::opDelayTime[index] * (sampleRate / 1000));
        samplesIntoPhase = 0;
        currentPhase = delayPhase;
    }
    float valueOf(juce::String& str)
    {
        return *tree->getRawParameterValue(str);
    }
    void triggerOff()
    {
        trigger = false;
        currentPhase = releasePhase;
        samplesIntoPhase = 0;
        samplesInPhase = ParamStatic::opReleaseTime[index] * (sampleRate / 1000);
        factor = exp((log(minLevel) - log(ParamStatic::opSustainLevel[index])) /samplesInPhase);
    }
    void setSampleRate(double value) {sampleRate = value;}
    float process(float input);
    envPhase getPhase() {return currentPhase;}
    double output;
private:
    //data
    static unsigned long long phaseSafe(unsigned long long input) //to avoid divide-by-zero errors
    {
        if(input > 0)
            return input;
        else
            return 1;
    }
    envPhase currentPhase;
    unsigned long long samplesIntoPhase;
    unsigned long long samplesInPhase;
    double factor;
    float minLevel = 0.00001f;
    double sampleRate;
    int index;
    bool trigger;
    juce::String delayId;
    juce::String attackId;
    juce::String holdId;
    juce::String decayId;
    juce::String sustainId;
    juce::String releaseId;
};
