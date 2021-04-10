/*
  ==============================================================================

    PatchManagerComponent.cpp
    Created: 10 Oct 2020 9:42:19pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "PatchManagerComponent.h"

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
    
    getPresetsFromFolder();
    
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

void PatchLoader::getPresetsFromFolder()
{
    patchSelector.clear();
    patchNames.clear();
    auto files = presetFolder.findChildFiles(juce::File::findFiles, true);
    auto numFiles = files.size();
    if(numFiles > 0)
    {
        for(int i = 0; i < numFiles; ++i)
        {
            auto currentFile = files.getUnchecked(i);
            std::unique_ptr<juce::XmlElement> currentXml = juce::parseXML(currentFile);
            if(currentXml != nullptr)
            {
                if(currentXml->hasAttribute("HexFmPatchName"))
                {
                    auto presetName = currentXml->getStringAttribute("HexFmPatchName");
                    patchNames.add(presetName);
                    patchSelector.addItem(presetName, 1 + i);
                    
                }
            }
    }
    if(patchNames.size() > 0)
    {
        patchSelector.setSelectedId(1);
        loadPreset(patchSelector.getText());
        printf("%d available patches\n", patchNames.size());
        printf("default patch found\n");
    }
}
}
void PatchLoader::savePreset(juce::String name)
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
        xml->writeTo(file);
        if(isNew)
        {
            patchNames.add(name);
            patchSelector.addItem(name, patchSelector.getNumItems() + 1);
        }
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
