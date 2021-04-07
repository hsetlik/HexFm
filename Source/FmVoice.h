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
    FmVoice(int numOperators, int index);
    ~FmVoice()
    {
        printf("Voice #: %d -- %d total jumps\n", voiceIndex, numJumps);
    }
    bool isActive()
    {
        for(Operator* i : operators)
        {
            if(i->envelope.getPhase() != DAHDSR::noteOff)
            {
                return true;
            }
        }
        return false;
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
        if(velocity == 0)
            clearCurrentNote();
    }
    void updateLfoTarget(std::atomic<float>* value, int lfo)
    {
        lfoBank[lfo]->currentTarget = *value;
    }
    void updateLfoRate(std::atomic<float>* value, int lfo)
    {
        lfoBank[lfo]->currentRate = *value;
    }
    void updateLfoWave(std::atomic<float>* value, int lfo)
    {
        lfoBank[lfo]->currentWaveType = *value;
    }
    void updateLfoLevel(std::atomic<float>* value, int lfo)
    {
        lfoBank[lfo]->currentLevel = *value;
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
    int voiceIndex;
    std::vector<std::vector<int>> routingParams;
    int numJumps;
    int operatorCount;
    float fundamental;
    juce::OwnedArray<LfoProcessor> lfoBank;
    juce::OwnedArray<Operator> operators;
    float opSample;
    float opSum;
    float lastOpSample;
    int lfoTarget;
    float lfoValue;
    float lfoMax;
    float lfoMin;
};


class FmSynth : public juce::Synthesiser
{
public:
    FmSynth(int operators, int lfos, int numVoices) : juce::Synthesiser(), numOperators(operators), numLfos(lfos)
    {
        for(int i = 0; i < numVoices; ++i)
        {
            addVoice(new FmVoice(numOperators, i));
        }
        addSound(new FmSound());
    }
    FmVoice* getFmVoice(int i)
    {
        return dynamic_cast<FmVoice*>(voices.getUnchecked(i));
    }
    juce::OwnedArray<juce::SynthesiserVoice>* voiceArray()
    {
        return &voices;
    }
private:
    int numOperators;
    int numLfos;
    
};
