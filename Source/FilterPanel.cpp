/*
  ==============================================================================

    FilterPanel.cpp
    Created: 15 Apr 2021 11:30:08pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "FilterPanel.h"
FilterPanel::FilterPanel() : cutoffLabel(&cutoffSlider, ""), resonanceLabel(&resonanceSlider, "")
{
    addAndMakeVisible(&cutoffSlider);
    cutoffSlider.setSliderStyle(juce::Slider::Rotary);
    cutoffSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 10, 4);
    cutoffSlider.setRange(0.0f, 15000.0f);
    cutoffSlider.setValue(2500.0f);
    addAndMakeVisible(&cutoffLabel);
    
    addAndMakeVisible(&resonanceSlider);
    resonanceSlider.setSliderStyle(juce::Slider::Rotary);
    resonanceSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    resonanceSlider.setRange(0.0f, 20.0f);
    resonanceSlider.setValue(1.0f);
    addAndMakeVisible(&resonanceLabel);
    
    addAndMakeVisible(&filterToggle);
    filterToggle.setButtonText("Filter Bypass");
    filterToggle.setClickingTogglesState(true);
    filterToggle.setColour(juce::TextButton::buttonColourId, UXPalette::lightRed);
    
    addAndMakeVisible(&filterType);
    filterType.setButtonText("Low Pass");
    filterType.setClickingTogglesState(true);
    filterType.addListener(this);
    
    cutoffSlider.setLookAndFeel(&look);
    resonanceSlider.setLookAndFeel(&look);
    filterToggle.setLookAndFeel(&look);
    filterType.setLookAndFeel(&look);
}

void FilterPanel::buttonClicked(juce::Button *b)
{
    auto str = filterType.getButtonText();
    if(str == "Low Pass")
    {
        filterType.setButtonText("High Pass");
    }
    else
    {
        filterType.setButtonText("Low Pass");
    }
}

void FilterPanel::resized()
{
    //Component is square!!
    auto dX = getWidth() / 10;
    filterType.setBounds(dX, dX, 3 * dX, dX);
    filterToggle.setBounds(6 * dX, dX, 3 * dX, dX);
    cutoffSlider.setBounds(dX, 4 * dX, 3 * dX, 4 * dX);
    cutoffLabel.setBounds(juce::Rectangle<int>(dX, 8 * dX, 3 * dX, dX).reduced(5));
    resonanceSlider.setBounds(6 * dX, 4 * dX, 3 * dX, 4 * dX);
    resonanceLabel.setBounds(juce::Rectangle<int>(6 * dX, 8 * dX, 3 * dX, dX).reduced(5));
}

void FilterPanel::paint(juce::Graphics &g)
{
    
}
