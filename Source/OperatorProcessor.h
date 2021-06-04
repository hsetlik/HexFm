/*
  ==============================================================================

    OperatorProcessor.h
    Created: 6 Oct 2020 1:04:48pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "maximilian.h"
#include "DAHDSR.h"
#include "LfoProcessor.h"
#include "WavetableProcessor.h"

class Operator
{
public:
    juce::AudioProcessorValueTreeState* tree;
    Operator(int opIndex, int voiceIndex, juce::AudioProcessorValueTreeState* t) : tree(t), lastOutputSample(0.0f), envelope(opIndex, tree), voice(voiceIndex), baseRatio(1.0f), index(opIndex)
    {
        auto iStr = juce::String(opIndex);
        panId = "panParam" + iStr;
        levelId = "levelParam" + iStr;
        ratioId = "ratioParam" + iStr;
        modIndexId = "indexParam" + iStr;
        workingRatio = baseRatio;
    }
    ~Operator()
    {
        //printf("Minimum ratio: %f\n", minRatio);
        //printf("Maximum ratio: %f\n", maxRatio);
    }
    float getValue(juce::String str)
    {
        return *tree->getRawParameterValue(str);
    }
    void updateParams()
    {
        envelope.updateParams();
        pan = getValue(panId);
        level = getValue(levelId);
        baseRatio = getValue(ratioId);
        workingRatio = baseRatio;
        modIndex = getValue(modIndexId);
    }
    int getIndex()
    {
        return index;
    }
    void cleanFreqOffset()
    {
        modOffset = 0.0f;
    }
    void modulateRatio(float value, int mode);
    
    bool isActive()
    {
        if(envelope.getPhase() == DAHDSR::noteOff)
            return false;
        else
            return true;
    }
    void updateSampleRate(double newRate)
    {
        envelope.setSampleRate(newRate);
        wtOsc.setSampleRate(newRate);
    }
    void updatePan()
    {
        panValue = (pan + 1.0f) / 2.0f;
        gainR = panValue;
        gainL = 1.0f - panValue;
        lastOutputL = lastOutputSample * gainL;
        lastOutputR = lastOutputSample * gainR;
    }
    void setAM(float value)
    {
        amplitudeMod = value;
    }
    static float lerp(float min, float max, float value)
    {
        return min + ((max - min) * value);
    }
    float sample(float fundamental);
    float lastOutputSample;
    float gainL;
    float gainR;
    float lastOutputL;
    float lastOutputR;
    bool isAudible = false;
    DAHDSR envelope;
    int voice;
    float modOffset;
    float rawSample;
    float panValue;
private:
    int index;
    VoiceOscillator wtOsc;
    juce::String panId;
    juce::String modIndexId;
    juce::String levelId;
    juce::String ratioId;
    juce::String amplitudeId;
    float pan;
    float baseRatio;
    float workingRatio;
    float modIndex;
    float level;
    float amplitudeMod;
    float fundamental;
    float maxRatioOffset;
    float minRatio;
    
};
