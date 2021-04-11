/*
  ==============================================================================

    PatchTypeData.h
    Created: 8 Apr 2021 3:34:43pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


enum class PatchType
{
    bass,
    lead,
    chord,
    pad
};
//keep both of these in the same order so strings can be accessed by int casting the enum value
static const std::vector<juce::String> PatchTypeStrings
{
    "Bass",
    "Lead",
    "Chord",
    "Pad"
};

struct PatchInfo
{
    juce::String name;
    PatchType type;
    PatchInfo(juce::String n, PatchType t) : name(n), type(t)
    {
        
    }
    juce::String getString()
    {
        return PatchTypeStrings[(int)type];
    }
};

struct PatchGroup
{
    PatchGroup(PatchType t) : type(t)
    {
        typeId = PatchTypeStrings[(int)type];
    }
    PatchType type;
    juce::String typeId;
    std::vector<juce::File> patches;
    void loadFilesForType(juce::File& folder)
    {
        auto allFiles = folder.findChildFiles(juce::File::TypesOfFileToFind::findFiles, true);
        for(auto f : allFiles)
        {
            std::unique_ptr<juce::XmlElement> currentXml = juce::parseXML(f);
            if(currentXml != nullptr)
            {
                if(currentXml->hasAttribute("HexFmPatchType"))
                {
                    auto checkType = currentXml->getStringAttribute("HexFmPatchType");
                    if(checkType == PatchTypeStrings[(int)type])
                        patches.push_back(f);
                    
                }
            }
        }
    }
    juce::String nameOfLastPatch()
    {
        if(patches.size() > 0)
        {
            std::unique_ptr<juce::XmlElement> currentXml = juce::parseXML(patches[patches.size() - 1]);
            if(currentXml->hasAttribute("HexFmPatchName"))
                return currentXml->getStringAttribute("HexFmPatchName");
        }
        return "";
    }
};

struct PatchLibrary
{
    PatchLibrary() :
    bassGroup(PatchType::bass),
    leadGroup(PatchType::lead),
    chordGroup(PatchType::chord),
    padGroup(PatchType::pad)
    {
        auto appFolder = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
        appFolder.setAsCurrentWorkingDirectory();
        auto patchFolder = appFolder.getChildFile("HexFM_Patches");
        if(patchFolder.exists() && patchFolder.isDirectory())
        {
            patchFolder.setAsCurrentWorkingDirectory();
        }
        else
        {
            patchFolder.createDirectory();
            patchFolder.setAsCurrentWorkingDirectory();
            printf("patch folder created\n");
        }
        presetFolder = patchFolder;
        bassGroup.loadFilesForType(presetFolder);
        leadGroup.loadFilesForType(presetFolder);
        chordGroup.loadFilesForType(presetFolder);
        padGroup.loadFilesForType(presetFolder);
    }
    PatchGroup* getGroup(juce::String& typeStr)
    {
        PatchType type = PatchType::lead;
        int idx = 0;
        for(auto s : PatchTypeStrings)
        {
            if(s == typeStr)
                type = (PatchType)idx;
            ++idx;
        }
        switch(type)
        {
            case PatchType::bass:
            {
                return &bassGroup;
            }
            case PatchType::lead:
            {
                return &leadGroup;
            }
            case PatchType::chord:
            {
                return &chordGroup;
            }
            case PatchType::pad:
            {
                return &padGroup;
            }
        }
    }
    PatchGroup bassGroup;
    PatchGroup leadGroup;
    PatchGroup chordGroup;
    PatchGroup padGroup;
    juce::File presetFolder;
};
