/*
  ==============================================================================

    ModulationGrid.h
    Created: 6 Oct 2020 1:08:49pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RGBColor.h"
#include "ParameterStructure.h"
#include "GlobalColor.h"

//==============================================================================


class ModulationToggle : public juce::ShapeButton
{
public:
    ModulationToggle(int source, int dest) :
    juce::ShapeButton("modButton", UXPalette::darkRed, UXPalette::darkRed, UXPalette::lightRed),
    sourceOp(source),
    destOp(dest)
    {
        shouldUseOnColours(true);
        setClickingTogglesState(true);
        buttonOnColor = UXPalette::lightRed;
        buttonOffColor = UXPalette::darkRed;
    }
    ~ModulationToggle() {}
    void paintButton(juce::Graphics& g, bool mouseOver, bool isMouseDown) override
    {
        g.setColour(buttonOffColor);
        if(getToggleState())
            g.setColour(buttonOnColor);
        auto rect = getLocalBounds().reduced(3);
        g.fillRect(rect);
    }
    void attach(juce::AudioProcessorValueTreeState* pTree)
    {
        auto sString = juce::String(sourceOp);
        auto dString = juce::String(destOp);
        auto paramId = sString + "to" + dString + "Param";
        attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(*pTree, paramId, *this));
    }
    int sourceOp;
    int destOp;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;
private:
    juce::Colour buttonOnColor = juce::Colours::white;
    juce::Colour buttonOffColor = juce::Colours::white;
};

class ModulationGrid  : public juce::Component, public juce::Button::Listener
{
public:
    ModulationGrid(int numOperators);
    ~ModulationGrid() override;
    void buttonClicked(juce::Button* b) override;
    void attachButtons(juce::AudioProcessorValueTreeState* pTree);
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    juce::Label sourceLabel;
    juce::Label destLabel;
    juce::OwnedArray<juce::OwnedArray<ModulationToggle>> buttons;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationGrid)
};
