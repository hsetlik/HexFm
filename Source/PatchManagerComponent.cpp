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
    int n = getWidth() / 18;
    
    patchSelector.setBounds(n, n / 2, 16 * n, 2 * n);
    
    lastPatchButton.setBounds(4 * n, 4 * n, 3 * n,  2 * n);
    nextPatchButton.setBounds(7 * n, 4 * n, 3 * n,  2 * n);
    saveButton.setBounds(12 * n, 4 * n, 3 * n, 2 * n);
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
                    if(i == 0)
                    {
                        loadPreset(presetName);
                    }
                }
            }
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
    else
    {
        
        
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
