/*
  ==============================================================================

    RGBColor.h
    Created: 25 Sep 2020 4:12:40pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
struct ColorCreator
{
    //without fourth argument, returns color with fully opaque alpha
    juce::Colour static RGBColor(int r, int g, int b)
    {
        return(juce::Colour(juce::uint8(r), juce::uint8(g), juce::uint8(b), juce::uint8(255)));
    }

    //fourth argument sets alpha
    juce::Colour static RGBColor(int r, int g, int b, int a)
    {
        return(juce::Colour(juce::uint8(r), juce::uint8(g), juce::uint8(b), juce::uint8(a)));
    }
    
    juce::Colour static blend(juce::Colour colorA, juce::Colour colorB, float ratio)
    {
        auto fRedA = colorA.getFloatRed();
        auto fGreenA = colorA.getFloatGreen();
        auto fBlueA = colorA.getFloatBlue();
        
        auto fRedB = colorB.getFloatRed();
        auto fGreenB = colorB.getFloatGreen();
        auto fBlueB = colorB.getFloatBlue();
        
        auto adjRedA = fRedA * (1.0f - ratio);
        auto adjGreenA = fGreenA * (1.0f - ratio);
        auto adjBlueA = fBlueA * (1.0f - ratio);
        
        auto adjRedB = fRedB * ratio;
        auto adjGreenB = fGreenB * ratio;
        auto adjBlueB = fBlueB * ratio;
        
        auto mixRed = (adjRedA + adjRedB) / 2.0f;
        auto mixGreen = (adjGreenA + adjGreenB) / 2.0f;
        auto mixBlue = (adjBlueA + adjBlueB) / 2.0f;
        
        int iRed = floor(mixRed * 255);
        int iGreen = floor(mixGreen * 255);
        int iBlue = floor(mixBlue * 255);
        
        return RGBColor(iRed, iGreen, iBlue);
    }
};
