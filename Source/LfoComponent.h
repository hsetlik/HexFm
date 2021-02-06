/*
  ==============================================================================

    LfoComponent.h
    Created: 9 Oct 2020 9:25:18am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "maximilian.h"
#include "RGBColor.h"

class LfoComponent : public juce::Component
{
public:
    //functions
    LfoComponent(int index);
    ~LfoComponent() {}
    void attachAll(juce::AudioProcessorValueTreeState* tree);
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    //data
    int lfoIndex;
    juce::ComboBox targetSelector;
    juce::ComboBox waveSelector;
    juce::Slider rateSlider;
    juce::Slider levelSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> targetAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveTypeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelAttach;
};
