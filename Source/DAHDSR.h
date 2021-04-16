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
        output = 0.0f;
        auto iStr = juce::String(ind);
        delayId = "delayParam" + iStr;
        attackId = "attackParam" + iStr;
        holdId = "holdParam" + iStr;
        decayId = "decayParam" + iStr;
        sustainId = "sustainParam" + iStr;
        releaseId = "releaseParam" + iStr;
    }
    ~DAHDSR() {}
    static envPhase nextPhase(envPhase input)
    {
        if(input != noteOff)
            return (envPhase)(input + 1);
        else
            return noteOff;
    }
    void triggerOn()
    {
        trigger = true;
        enterPhase(delayPhase);
    }
    float valueOf(juce::String& str)
    {
        return *tree->getRawParameterValue(str);
    }
    std::atomic<float>* ptrValue(juce::String str)
    {
        return tree->getRawParameterValue(str);
    }
    void updateParams()
    {
        delayTime = valueOf(delayId);
        attackTime = valueOf(attackId);
        holdTime = valueOf(holdId);
        decayTime = valueOf(decayId);
        sustainLevel = valueOf(sustainId);
        releaseTime = valueOf(releaseId);
    }
    void triggerOff()
    {
        trigger = false;
        enterPhase(releasePhase);
    }
    void updatePhase()
    {
        if(samplesIntoPhase > samplesInPhase || samplesInPhase < 1)
        {
            enterPhase(nextPhase(currentPhase));
        }
    }
    void enterPhase(envPhase newPhase);
    float factorFor(float startLevel, float endLevel, float lengthMs);
    void setSampleRate(double value) {sampleRate = value;}
    float process(float input);
    envPhase getPhase() {return currentPhase;}
    bool isActive()
    {
        if(currentPhase == envPhase::noteOff)
            return false;
        return true;
    }
    float output;
private:
    //data
    static unsigned long long phaseSafe(unsigned long long input) //to avoid divide-by-zero errors
    {
        if(input > 0)
            return input;
        return 1;
    }
    envPhase currentPhase;
    unsigned long long samplesIntoPhase;
    unsigned long long samplesInPhase;
    float factor;
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
private:
    float delayTime = 0.0f;
    float attackTime = 20.0f;
    float holdTime = 0.0f;
    float decayTime = 100.0f;
    float sustainLevel = 0.6f;
    float releaseTime = 40.0f;
    
    float startLevel;
    float endLevel;
};
