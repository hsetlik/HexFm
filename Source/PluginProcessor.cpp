/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout createLayout(int numOperators)
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    for(int i = 0; i < numOperators; ++i)
    {
        juce::String iStr = juce::String(i);
        //strings and parameters for the operator
        auto ratioId = "ratioParam" + iStr;
        auto ratioName = "Operator " + iStr + " ratio";
        auto levelId = "levelParam" + iStr;
        auto levelName = "Operator " + iStr + " level";
        auto indexId = "indexParam" + iStr;
        auto indexName = "Operator " + iStr + " Mod Index";
        auto outputId = "audibleParam" + iStr;
        auto outputName = "Operator " + iStr + " audible";
        auto panId = "panParam" + iStr;
        auto panName = "Operator " + iStr + " pan";
        juce::NormalisableRange<float> ratioRange(0.0f, 10.0f, 0.01f, 0.5f);
        ratioRange.setSkewForCentre(1.0f);
        layout.add(std::make_unique<juce::AudioParameterFloat>(ratioId, ratioName, ratioRange, 1.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(levelId, levelName, 0.0f, 1.0f, 1.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(indexId, indexName, 0.0f, 500.0f, 0.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(panId, panName, -1.0f, 1.0f, 0.0f));
        layout.add(std::make_unique<juce::AudioParameterBool>(outputId, outputName, false));
        //and for the envelope
        auto delayId = "delayParam" + iStr;
        auto delayName = "Operator " + iStr + " delay";
        auto attackId = "attackParam" + iStr;
        auto attackName = "Operator " + iStr + " attack";
        auto holdId = "holdParam" + iStr;
        auto holdName = "Operator " + iStr + " hold";
        auto decayId = "decayParam" + iStr;
        auto decayName = "Operator " + iStr + " decay";
        auto sustainId = "sustainParam" + iStr;
        auto sustainName = "Operator " + iStr + " sustain";
        auto releaseId = "releaseParam" + iStr;
        auto releaseName = "Operator " + iStr + " release";
        juce::NormalisableRange<float> timeRange1(1.0f, 20000.0f, 0.1f, 0.5f);
        timeRange1.setSkewForCentre(1000.0f);
        juce::NormalisableRange<float> timeRange2(1.0f, 20000.0f, 0.1f, 0.5f);
        timeRange2.setSkewForCentre(5000.0f);
        layout.add(std::make_unique<juce::AudioParameterFloat>(delayId, delayName, timeRange2, 0.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(attackId, attackName, timeRange1, 20.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(holdId, holdName, timeRange1, 0.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(decayId, decayName, timeRange2, 100.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(sustainId, sustainName, 0.0f, 1.0f , 0.6f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(releaseId, releaseName, timeRange2, 40.0f));
        //bools for modulation paths
        for(int n = 0; n < numOperators; ++n)
        {
            juce::String nStr = juce::String(n);
            auto modId = iStr + "to" + nStr + "Param";
            auto modName = "Operator " + iStr + " to " + nStr;
            layout.add(std::make_unique<juce::AudioParameterBool>(modId, modName, false));
        }
    }
    for(int i = 0; i < 4; ++i)
    {
        auto iStr = juce::String(i);
        auto rateId = "lfoRateParam" + iStr;
        auto rateName = "LFO " + iStr + " Rate";
        auto levelId = "lfoLevelParam" + iStr;
        auto levelName = "LFO " + iStr + " Level";
        auto waveId = "lfoWaveParam" + iStr;
        auto waveName = "LFO " + iStr + " Waveform";
        auto targetId = "lfoTargetParam" + iStr;
        auto targetName = "LFO " + iStr + " Target";
        auto modeId = "lfoRatioModeParam" + iStr;
        auto modeName = "LFO " + iStr + "ratio modulation mode";
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(rateId, rateName, 0.0f, 20.0f , 1.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(levelId, levelName, 0.0f, 1.0f , 1.0f));
        
        juce::StringArray targets;
        targets.add("No target");
        for(int n = 0; n < numOperators; ++n)
        {
            auto nStr = juce::String(n + 1);
            auto opLevel = "Operator " + nStr + " level";
            auto opRatio = "Operator " + nStr + "level";
            targets.add(opRatio);
            targets.add(opLevel);
        }
        juce::StringArray waveTypes;
        waveTypes.add("Sine");
        waveTypes.add("Triangle");
        waveTypes.add("Square");
        waveTypes.add("Saw");
        waveTypes.add("Random");
        
        juce::StringArray ratioModes;
        ratioModes.add("Up");
        ratioModes.add("Both");
        ratioModes.add("Down");
        
        layout.add(std::make_unique<juce::AudioParameterChoice>(modeId, modeName, ratioModes, 0));
        layout.add(std::make_unique<juce::AudioParameterChoice>(targetId, targetName, targets, 0));
        layout.add(std::make_unique<juce::AudioParameterChoice>(waveId, waveName, waveTypes, 0));
    }
    return layout;
}

//==============================================================================
HexFmAudioProcessor::HexFmAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),  tree(*this, nullptr, "synthParams", createLayout(numOperators)), synth(6, 4, 6)
#endif
{
    for(int i = 0; i < 4; ++i)
    {
        auto iStr = juce::String(i);
        lfoRateIds.push_back("lfoRateParam" + iStr);
        lfoWaveIds.push_back("lfoWaveParam" + iStr);
        lfoTargetIds.push_back("lfoTargetParam" + iStr);
        lfoLevelIds.push_back("lfoLevelParam" + iStr);
        lfoRatioModeIds.push_back("lfoRatioModeParam" + iStr);
    }
    for(int i = 0; i < numOperators; ++i)
    {
        auto iStr = juce::String(i);
        ratioIds.push_back("ratioParam" + iStr);
        levelIds.push_back("levelParam" + iStr);
        modIndexIds.push_back("indexParam" + iStr);
        audibleIds.push_back("audibleParam" + iStr);
        panIds.push_back("panParam" + iStr);
        
        delayIds.push_back("delayParam" + iStr);
        attackIds.push_back("attackParam" + iStr);
        holdIds.push_back("holdParam" + iStr);
        decayIds.push_back("decayParam" + iStr);
        sustainIds.push_back("sustainParam" + iStr);
        releaseIds.push_back("releaseParam" + iStr);
        std::vector<juce::String> routing;
        for(int n = 0; n < numOperators; ++n)
        {
            auto nStr = juce::String(n);
            routing.push_back(iStr + "to" + nStr + "Param");
        }
        routingIds.push_back(routing);
    }
}

//======================================================================
HexFmAudioProcessor::~HexFmAudioProcessor()
{
}

//==============================================================================
const juce::String HexFmAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HexFmAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HexFmAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HexFmAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HexFmAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HexFmAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HexFmAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HexFmAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HexFmAudioProcessor::getProgramName (int index)
{
    return {};
}

void HexFmAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HexFmAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);
    synth.setCurrentPlaybackSampleRate(sampleRate);
    maxiSettings::setup((int)sampleRate, 2, samplesPerBlock);
    for(juce::SynthesiserVoice* i : *synth.voiceArray())
    {
        auto* voice = dynamic_cast<FmVoice*>(i);
        voice->setSampleRate(sampleRate);
    }
}

void HexFmAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HexFmAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HexFmAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    ParamStatic::setRouting(&tree, routingIds);
    
    for(lfoIndex = 0; lfoIndex < TOTAL_LFOS; ++lfoIndex)
     {
         ParamStatic::lfoTarget[lfoIndex] = (int)*tree.getRawParameterValue(lfoTargetIds[lfoIndex]);
         ParamStatic::lfoWave[lfoIndex] = (int)*tree.getRawParameterValue(lfoWaveIds[lfoIndex]);
         ParamStatic::lfoRate[lfoIndex].setFrom(tree.getRawParameterValue(lfoRateIds[lfoIndex]));
         ParamStatic::lfoLevel[lfoIndex] = *tree.getRawParameterValue(lfoLevelIds[lfoIndex]);
         ParamStatic::lfoRatioMode[lfoIndex] = *tree.getRawParameterValue(lfoRatioModeIds[lfoIndex]);
     }
    for(opIndex = 0; opIndex < TOTAL_OPERATORS; ++opIndex)
    {
        ParamStatic::opRatio[opIndex].setFrom(tree.getRawParameterValue(ratioIds[opIndex]));
        ParamStatic::opLevel[opIndex].setFrom(tree.getRawParameterValue(levelIds[opIndex]));
        ParamStatic::opModIndex[opIndex].setFrom(tree.getRawParameterValue(modIndexIds[opIndex]));
        ParamStatic::opAudible[opIndex] = (int)*tree.getRawParameterValue(audibleIds[opIndex]);
        ParamStatic::opPanValue[opIndex].setFrom(tree.getRawParameterValue(panIds[opIndex]));
        
        ParamStatic::opDelayTime[opIndex].setFrom(tree.getRawParameterValue(delayIds[opIndex]));
        ParamStatic::opAttackTime[opIndex].setFrom(tree.getRawParameterValue(attackIds[opIndex]));
        ParamStatic::opHoldTime[opIndex].setFrom(tree.getRawParameterValue(holdIds[opIndex]));
        ParamStatic::opDecayTime[opIndex].setFrom(tree.getRawParameterValue(decayIds[opIndex]));
        ParamStatic::opSustainLevel[opIndex].setFrom(tree.getRawParameterValue(sustainIds[opIndex]));
        ParamStatic::opReleaseTime[opIndex].setFrom(tree.getRawParameterValue(releaseIds[opIndex]));
    }
    buffer.clear();
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool HexFmAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HexFmAudioProcessor::createEditor()
{
    return new HexFmAudioProcessorEditor (*this);
}

//==============================================================================
void HexFmAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = tree.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void HexFmAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
            if (xmlState.get() != nullptr)
                if (xmlState->hasTagName (tree.state.getType()))
                    tree.replaceState (juce::ValueTree::fromXml (*xmlState));
}
//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HexFmAudioProcessor();
}
