/*
  ==============================================================================

    FmVoice.h
    Created: 6 Oct 2020 3:28:58pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AlgorithmProcessor.h"
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
    static std::vector<float> opLevel;
    static std::vector<float> opModIndex;
    
    static std::vector<float> lfoRate;
    static std::vector<float> lfoLevel;
    static std::vector<int> lfoTarget;
    static std::vector<int> lfoWave;
};

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
        fundamental = convert::mtof(midiNoteNumber - 12);
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
    void setParameters
                     (
                     int operatorIndex,
                     std::atomic<float>* ratio,
                     std::atomic<float>* level,
                     std::atomic<float>* modIndex,
                     std::atomic<float>* isAudible,
                     std::atomic<float>* envDelay,
                     std::atomic<float>* envAttack,
                     std::atomic<float>* envHold,
                     std::atomic<float>* envDecay,
                     std::atomic<float>* envSustain,
                     std::atomic<float>* envRelease
                     )
    {
        operators[operatorIndex]->setRatio(*ratio);
        operators[operatorIndex]->setLevel(*level);
        operators[operatorIndex]->setModIndex(*modIndex);
        operators[operatorIndex]->setAudible(*isAudible);
        operators[operatorIndex]->envelope.setDelay(*envDelay);
        operators[operatorIndex]->envelope.setAttack(*envAttack);
        operators[operatorIndex]->envelope.setHold(*envHold);
        operators[operatorIndex]->envelope.setDecay(*envDecay);
        operators[operatorIndex]->envelope.setSustain(*envSustain);
        operators[operatorIndex]->envelope.setRelease(*envRelease);
    }
    void setSampleRate(double newRate)
    {
        for(Operator* i : operators)
        {
            i->envelope.setSampleRate(newRate);
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
    //TODO: refactor this renderVoices() function as nuclear option
    //void renderVoices(juce::AudioBuffer<float> &buffer, int startSample, int numSamples) override;
private:
    int numOperators;
    int numLfos;
};
