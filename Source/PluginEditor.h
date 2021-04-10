/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OperatorComponent.h"
#include "ModulationGrid.h"
#include "RGBColor.h"
#include "GlobalColor.h"
#include "LfoGroupComponent.h"
#include "PatchManagerComponent.h"
#include "PatchSavingDialogBox.h"

//==============================================================================
/**
*/
class HexFmAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HexFmAudioProcessorEditor (HexFmAudioProcessor&);
    ~HexFmAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void paint(juce::Graphics &g) override;
    void updatePresetSelection();

private:
    juce::TooltipWindow tipWindow;
    juce::OwnedArray<OperatorComponent> allOps;
    ModulationGrid modGrid;
    LfoGroupComponent lfoGroup;
    PatchLoader patchLoader;
    PatchDialogBox saveDialog;
    AlgorithmGraph algGraph;
    juce::Colour operatorColor;
    int dY;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HexFmAudioProcessor& audioProcessor;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HexFmAudioProcessorEditor)
};
