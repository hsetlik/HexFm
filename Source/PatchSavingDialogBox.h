/*
  ==============================================================================

    PatchSavingDialogBox.h
    Created: 12 Oct 2020 11:01:32am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PatchManagerComponent.h"
#include "PatchTypeData.h"
#include "GlobalColor.h"

class PatchDialogBox : public juce::Component, juce::Button::Listener
{
public:
    PatchDialogBox(PatchLoader* loader);
    ~PatchDialogBox() {}
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void paint(juce::Graphics& g) override;
    juce::String getNewPatchName()
    {
        return nameField.getText();
    }
    juce::String getNewPatchType()
    {
        return typeBox.getText();
    }
    juce::TextButton savePatchButton;
    juce::TextButton cancelButton;
    juce::ComboBox typeBox;
private:
    PatchLoader* patchLoader;
    juce::TextEditor nameField;
};
