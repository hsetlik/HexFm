/*
  ==============================================================================

    PatchManagerComponent.h
    Created: 10 Oct 2020 9:42:19pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class PatchLoader : public juce::Component, juce::Button::Listener, juce::ComboBox::Listener
{
public:
    PatchLoader(HexFmAudioProcessor* proc, juce::Component* patchDlg);
    ~PatchLoader() {}
    void resized() override;
    void loadNames(juce::StringArray patchNames)
    {
        patchSelector.addItemList(patchNames, 1);
    }
    //TODO:
    void getPresetsFromFolder();
    void savePreset(juce::String name);
    void loadPreset(juce::String name);
    void comboBoxChanged(juce::ComboBox* box) override;
    //
    juce::ComboBox* getSelectorBox()
    {
        return &patchSelector;
    }
    juce::String getCurrentPresetName()
    {
        return patchSelector.getText();
    }
    void buttonClicked(juce::Button* button) override
    {
        auto totalItems = patchSelector.getNumItems();
        auto currentIndex = patchSelector.getSelectedItemIndex();
        auto maxIncrease = totalItems - currentIndex;
        if(button == &nextPatchButton && maxIncrease > 0)
        {
            patchSelector.setSelectedItemIndex(currentIndex + 1);
        }
        else if(button == &lastPatchButton && (currentIndex - 1) >= 0)
        {
            patchSelector.setSelectedItemIndex(currentIndex - 1);
        }
        else if(button == &saveButton)
        {
            saveDialogComponent->setEnabled(true);
            saveDialogComponent->setVisible(true);
            saveDialogComponent->toFront(true);
            saveDialogComponent->resized();
        }
    }
    juce::ComboBox patchSelector;
    
    HexFmAudioProcessor* processor;
private:
    juce::File presetFolder;
    juce::StringArray patchNames;
    juce::Component* saveDialogComponent;
    juce::TextButton saveButton;
    juce::TextButton nextPatchButton;
    juce::TextButton lastPatchButton;
    juce::StringArray displayPatchNames;
};

