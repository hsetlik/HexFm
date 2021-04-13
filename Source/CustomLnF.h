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
    void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider &) override;
};

class LnF2 : public juce::LookAndFeel_V4
{
public:
    LnF2()
    {
        setDefaultSansSerifTypefaceName("Roboto Light Italic");
    }
};


class OutputButtonLnF : public juce::LookAndFeel_V4
{
public:
    OutputButtonLnF()
    {
        setDefaultSansSerifTypefaceName("");
    }
    
    juce::Font getTextButtonFont(juce::TextButton &, int buttonHeight) override
    {
        return juce::Font("Bebas Neue", 15.0f, 0);
    }
};
