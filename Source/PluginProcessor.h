/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FmVoice.h"
#include "LayoutCreator.h"


const int numOperators = 6;
const int numVoices = 8;

//==============================================================================
class HexFmAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    HexFmAudioProcessor();
    ~HexFmAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================
    
    //====================================================
    juce::AudioProcessorValueTreeState tree;
    FmVoice* thisVoice;
private:
    std::vector<juce::String> lfoTargetIds;
    std::vector<juce::String> lfoWaveIds;
    std::vector<juce::String> lfoRateIds;
    std::vector<juce::String> lfoLevelIds;
    
    std::vector<juce::String> ratioIds;
    std::vector<juce::String> levelIds;
    std::vector<juce::String> modIndexIds;
    std::vector<juce::String> audibleIds;
    
    std::vector<juce::String> delayIds;
    std::vector<juce::String> attackIds;
    std::vector<juce::String> holdIds;
    std::vector<juce::String> decayIds;
    std::vector<juce::String> sustainIds;
    std::vector<juce::String> releaseIds;
    std::vector<std::vector<juce::String>> routingIds;
    
    int lfoIndex;
    int opIndex;
    int voiceIndex;
    FmSynth synth;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HexFmAudioProcessor)
};
