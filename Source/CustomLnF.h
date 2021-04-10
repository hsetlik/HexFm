/*
  ==============================================================================

    CustomLnF.h
    Created: 7 Apr 2021 12:21:29am
    Author:  Hayden Setlik

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "GlobalColor.h"
class LnF1 : public juce::LookAndFeel_V4
{
public:
    LnF1()
    {
    }
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& s) override;
};
