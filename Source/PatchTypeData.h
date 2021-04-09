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

struct PatchInfo {
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
