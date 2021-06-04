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
#include "GlobalColor.h"
#include "CustomLnF.h"
class LfoModeSelector : public juce::Component, juce::Button::Listener
{
public:
    LfoModeSelector(int index);
    ~LfoModeSelector() {}
    void attach(juce::AudioProcessorValueTreeState* tree);
    void buttonClicked(juce::Button* b) override;
    void paint(juce::Graphics& g) override
    {
        
    }
    void initialize()
    {
        upButton.triggerClick();
    }
    void resized() override;
private:
    int modeSelectorIndex;
    juce::ImageButton upButton;
    juce::ImageButton bothButton;
    juce::ImageButton downButton;
    juce::Image upOffImg;
    juce::Image upOnImg;
    juce::Image bothOffImg;
    juce::Image bothOnImg;
    juce::Image downOffImg;
    juce::Image downOnImg;
    juce::ComboBox choiceHandler;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttach;
};

class LfoComponent : public juce::Component, juce::ComboBox::Listener
{
public:
    //functions
    LfoComponent(int index);
    ~LfoComponent()
    {
        rateSlider.setLookAndFeel(nullptr);
        levelSlider.setLookAndFeel(nullptr);
        waveSelector.setLookAndFeel(nullptr);
        targetSelector.setLookAndFeel(nullptr);
    }
    void attachAll(juce::AudioProcessorValueTreeState* tree);
    void comboBoxChanged(juce::ComboBox* c) override;
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    //data
    int lfoIndex;
    juce::ComboBox targetSelector;
    juce::ComboBox waveSelector;
    juce::Slider rateSlider;
    juce::Slider levelSlider;
    
    LfoModeSelector selector;
    LnF1 look;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> targetAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveTypeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelAttach;
};
