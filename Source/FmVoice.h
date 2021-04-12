/*
  ==============================================================================

    FmVoice.h
    Created: 6 Oct 2020 3:28:58pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "OperatorProcessor.h"


class FmSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int /*midiNoteNumber*/) //just plays this sound for any midi note
    {
        return true;
    }
    bool appliesToChannel(int /*midiChannel*/) //plays the sound on both channels
    {
        return true;
    }
};
class FmVoice : public juce::SynthesiserVoice
{
public:
    juce::AudioProcessorValueTreeState* tree;
    FmVoice(int numOperators, int index, juce::AudioProcessorValueTreeState* t);
    ~FmVoice()
    {
        printf("Voice #: %d -- %d total jumps\n", voiceIndex, numJumps);
    }
    bool canPlaySound(juce::SynthesiserSound* sound)
    {
        return dynamic_cast<FmSound*>(sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber,
                    float velocity,
                    juce::SynthesiserSound *sound,
                    int currentPitchWheelPosition)
    {
        fundamental = convert::mtof(midiNoteNumber);
        for(Operator* i : operators)
        {
            i->envelope.triggerOn();
        }
    }
    void stopNote (float velocity, bool allowTailOff)
    {
        for(Operator* i : operators)
        {
            i->envelope.triggerOff();
        }
        allowTailOff = true;
        if(velocity == 0 || !isActive())
            clearCurrentNote();
        
    }
    void applyLfo(int index);
    void setRoutingFromGrid(juce::AudioProcessorValueTreeState* pTree, std::vector<std::vector<juce::String>> grid);
    void setSampleRate(double newRate)
    {
        for(Operator* i : operators)
        {
            i->updateSampleRate(newRate);
        }
    }
    void pitchWheelMoved(int newPitchWheelVal) {}
    //=============================================
    void controllerMoved(int controllerNumber, int controllerValue) {}
    //===============================================
    void aftertouchChanged (int newAftertouchValue) {}
    //==============================================
    void channelPressureChanged (int newChannelPressureValue) {}
    //===============================================
    void renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples);
    //==============================================
    void setCurrentPlaybackSampleRate (double newRate)
    {
        setSampleRate(newRate);
    }
    float getValue(juce::String str)
    {
        return *tree->getRawParameterValue(str);
    }
    bool isActive()
    {
        for(auto op : operators)
        {
            if(op->envelope.isActive())
                return true;
        }
        return false;
    }
    void updateParams();
    int voiceIndex;
    int numJumps;
    int operatorCount;
    float fundamental;
    juce::OwnedArray<LfoProcessor> lfoBank;
    juce::OwnedArray<Operator> operators;
    float opSample;
    float opSum;
    float lastOpSample;
    int lfoTarget;
    float sumL;
    float sumR;
    float lfoValue;
    float lfoMax;
    float lfoMin;
private:
    int op1Index, op2Index;
    juce::String opRoutingIds[TOTAL_OPERATORS][TOTAL_OPERATORS];
    juce::String opAudibleIds[TOTAL_OPERATORS];
    int opRouting[TOTAL_OPERATORS][TOTAL_OPERATORS];
    int opAudible[TOTAL_OPERATORS];
};


class FmSynth : public juce::Synthesiser
{
public:
    juce::AudioProcessorValueTreeState* tree;
    FmSynth(int operators, int lfos, int nVoices, juce::AudioProcessorValueTreeState* t) : juce::Synthesiser(), tree(t), numOperators(operators), numLfos(lfos), numVoices(nVoices)
    {
        for(int i = 0; i < numVoices; ++i)
        {
            addVoice(new FmVoice(numOperators, i, tree));
        }
        addSound(new FmSound());
    }
    FmVoice* getFmVoice(int& i)
    {
        return dynamic_cast<FmVoice*>(voices.getUnchecked(i));
    }
    juce::OwnedArray<juce::SynthesiserVoice>* voiceArray()
    {
        return &voices;
    }
    void updateParams()
    {
        for(idx = 0; idx < numVoices; ++idx)
        {
            getFmVoice(idx)->updateParams();
        }
    }
private:
    int idx;
    int numOperators;
    int numLfos;
    int numVoices;
};
