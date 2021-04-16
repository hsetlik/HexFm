/*
  ==============================================================================

    FilterPanel.h
    Created: 15 Apr 2021 11:30:08pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "RGBColor.h"
#include "GlobalColor.h"
#include "CustomLnF.h"
#include "SliderSubclasses.h"

class FilterPanel : public juce::Component, juce::Button::Listener
{
public:
    FilterPanel();
    ~FilterPanel()
    {
        cutoffSlider.setLookAndFeel(nullptr);
        resonanceSlider.setLookAndFeel(nullptr);
        filterToggle.setLookAndFeel(nullptr);
        filterType.setLookAndFeel(nullptr);
    }
    void attach(juce::AudioProcessorValueTreeState* t)
    {
        cutoffAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*t, "cutoffParam", cutoffSlider));
        resonanceAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*t, "resonanceParam", resonanceSlider));
        toggleAttach.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(*t, "filterToggleParam", filterToggle));
        typeAttach.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(*t, "filterTypeParam", filterType));
    }
    void buttonClicked(juce::Button* b) override;
    void resized() override;
    void paint(juce::Graphics& g) override;
    juce::Slider cutoffSlider;
    juce::Slider resonanceSlider;
    EnvLabel cutoffLabel;
    EnvLabel resonanceLabel;
    juce::TextButton filterToggle;
    juce::TextButton filterType;
private:
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> toggleAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> typeAttach;
    LnF1 look;
};
