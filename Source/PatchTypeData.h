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
    }
    PatchGroup bassGroup;
    PatchGroup leadGroup;
    PatchGroup chordGroup;
    PatchGroup padGroup;
    juce::File presetFolder;
};
