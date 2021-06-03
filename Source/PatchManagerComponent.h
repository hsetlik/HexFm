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
#include "PatchTypeData.h"

class PatchSelector : public juce::ComboBox
{
public:
    PatchSelector() : lib(std::make_unique<PatchLibrary>())
    {
        initialize();
        lastPatchNames = patchNames;
    }
    ~PatchSelector(){}
    void initialize();
    void selectNewest();
    void addPatch(std::unique_ptr<juce::XmlElement>& element, int idNum);
    void reInitList()
    {
        clear();
        lib.reset(new PatchLibrary());
        initialize();
        selectNewest();
        
    }
    int getIndexWithText(juce::String text)
    {
        for(int i = 0; i < getNumItems(); ++i)
        {
            if(getItemText(i) == text)
                return i;
        }
        return getNumItems() - 1;
    }
    juce::StringArray patchNames;
    juce::StringArray lastPatchNames;
private:
    std::unique_ptr<PatchLibrary> lib;
};

class PatchLoader : public juce::Component, juce::Button::Listener, juce::ComboBox::Listener
{
public:
    PatchLoader(HexFmAudioProcessor* proc, juce::Component* patchDlg);
    ~PatchLoader() {}
    void resized() override;
    void savePreset(juce::String name, juce::String type);
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
    juce::String getCurrentPresetType()
    {
        auto fArray = presetFolder.findChildFiles(juce::File::TypesOfFileToFind::findFiles, true);
        juce::String output = "Bass";
        auto pName = getCurrentPresetName();
        for(auto f : fArray)
        {
            std::unique_ptr<juce::XmlElement> currentXml = juce::parseXML(f);
            if(currentXml != nullptr)
            {
                if(currentXml->hasAttribute("HexFmPatchType"))
                {
                    auto checkName = currentXml->getStringAttribute("HexFmPatchName");
                    if(checkName == pName)
                    {
                        output = currentXml->getStringAttribute("HexFmPatchType");
                    }
                }
            }
        }
        return output;
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
    
    
    PatchSelector patchSelector;
    
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

