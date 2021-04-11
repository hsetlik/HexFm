/*
  ==============================================================================

    PatchManagerComponent.cpp
    Created: 10 Oct 2020 9:42:19pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "PatchManagerComponent.h"

void PatchSelector::initialize()
{
    int nextId = 1;
    if(lib->bassGroup.patches.size() > 0)
    {
        addSectionHeading("Bass");
        addSeparator();
        for(auto f : lib->bassGroup.patches)
        {
            std::unique_ptr<juce::XmlElement> currentXml = juce::parseXML(f);
            if(currentXml != nullptr)
            {
                if(currentXml->hasAttribute("HexFmPatchName"))
                {
                    auto presetName = currentXml->getStringAttribute("HexFmPatchName");
                    patchNames.add(presetName);
                    addItem(presetName, nextId);
                    ++nextId;
                }
            }
        }
    }
    
    if(lib->leadGroup.patches.size() > 0)
    {
        addSectionHeading("Lead");
        addSeparator();
        for(auto f : lib->leadGroup.patches)
        {
            std::unique_ptr<juce::XmlElement> currentXml = juce::parseXML(f);
            if(currentXml != nullptr)
            {
                if(currentXml->hasAttribute("HexFmPatchName"))
                {
                    auto presetName = currentXml->getStringAttribute("HexFmPatchName");
                    patchNames.add(presetName);
                    addItem(presetName, nextId);
                    ++nextId;
                }
            }
        }
    }
    
    if(lib->chordGroup.patches.size() > 0)
    {
        addSectionHeading("Chord");
        addSeparator();
        for(auto f : lib->chordGroup.patches)
        {
            std::unique_ptr<juce::XmlElement> currentXml = juce::parseXML(f);
            if(currentXml != nullptr)
            {
                if(currentXml->hasAttribute("HexFmPatchName"))
                {
                    auto presetName = currentXml->getStringAttribute("HexFmPatchName");
                    patchNames.add(presetName);
                    addItem(presetName, nextId);
                    ++nextId;
                }
            }
        }
    }
    
    if(lib->padGroup.patches.size() > 0)
    {
        addSectionHeading("Pad");
        addSeparator();
        for(auto f : lib->padGroup.patches)
        {
            std::unique_ptr<juce::XmlElement> currentXml = juce::parseXML(f);
            if(currentXml != nullptr)
            {
                if(currentXml->hasAttribute("HexFmPatchName"))
                {
                    auto presetName = currentXml->getStringAttribute("HexFmPatchName");
                    patchNames.add(presetName);
                    addItem(presetName, nextId);
                    ++nextId;
                }
            }
        }
    }
}


PatchLoader::PatchLoader(HexFmAudioProcessor* proc, juce::Component* patchDlg) :
processor(proc),
saveDialogComponent(patchDlg)
{
    auto appFolder = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    appFolder.setAsCurrentWorkingDirectory();
    auto patchFolder = appFolder.getChildFile("HexFM_Patches");
    if(patchFolder.exists() && patchFolder.isDirectory())
    {
        printf("patch folder exists\n");
        patchFolder.setAsCurrentWorkingDirectory();
    }
    else
    {
        patchFolder.createDirectory();
        patchFolder.setAsCurrentWorkingDirectory();
        printf("patch folder created\n");
    }
    presetFolder = patchFolder;
    
    //auto fldr = presetFolder.getFileName().toUTF8();
    //printf("Presets are at: %s\n", fldr);
    
    addAndMakeVisible(&patchSelector);
    addAndMakeVisible(&nextPatchButton);
    addAndMakeVisible(&lastPatchButton);
    addAndMakeVisible(&saveButton);
    
    nextPatchButton.addListener(this);
    lastPatchButton.addListener(this);
    saveButton.addListener(this);
    patchSelector.addListener(this);
    
    nextPatchButton.setButtonText(">");
    lastPatchButton.setButtonText("<");
    saveButton.setButtonText("Save");
}

void PatchLoader::resized()
{
    auto dY = getHeight() / 8;
    //aspect ratio is 2/19 -> 6/57
    //component width is 57 * dY
    lastPatchButton.setBounds(dY, dY, 4 * dY, 4 * dY);
    nextPatchButton.setBounds(5 * dY, dY, 4 * dY, 4 * dY);
    patchSelector.setBounds(11 * dY, dY, 30 * dY, 4 * dY);
    saveButton.setBounds(44 * dY, dY, 6 * dY, 4 * dY);
}

void PatchLoader::savePreset(juce::String name, juce::String type)
{
    auto fileName = juce::File::createLegalFileName(name);
    if(presetFolder.exists())
    {
        auto file = presetFolder.getChildFile(fileName);
        bool isNew = false;
        if(!file.existsAsFile())
        {
            file.create();
            isNew = true;
        }
        auto state = processor->tree.copyState();
        std::unique_ptr<juce::XmlElement> xml = state.createXml();
        xml->setAttribute("HexFmPatchName", name);
        xml->setAttribute("HexFmPatchType", type);
        xml->writeTo(file);
        patchSelector.reInitList();
    }
}
void PatchLoader::loadPreset(juce::String presetName)
{
    auto presetFiles = presetFolder.findChildFiles(juce::File::findFiles, true);
    if(presetFiles.size() > 0)
    {
        for(int i = 0; i < presetFiles.size(); ++i)
        {
            auto currentFile = presetFiles.getUnchecked(i);
            std::unique_ptr<juce::XmlElement> currentXml = juce::parseXML(currentFile);
            if(currentXml != nullptr)
            {
                if(currentXml->hasAttribute("HexFmPatchName"))
                {
                    auto checkName = currentXml->getStringAttribute("HexFmPatchName");
                    if(checkName == presetName)
                    {
                        auto* vTree = &processor->tree;
                        vTree->replaceState(juce::ValueTree::fromXml(*currentXml));
                    }
                }
            }
        }
    }
}
    
void PatchLoader::comboBoxChanged(juce::ComboBox* box)
{
    if(box == &patchSelector)
    {
        loadPreset(patchSelector.getText());
    }
}
