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
        buttonFont = UXPalette::bebasNeue(8.0f);
    }
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& s) override;
    void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider &) override;
    void drawButtonBackground (juce::Graphics &, juce::Button &b, const juce::Colour &backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    void drawButtonText(juce::Graphics& g, juce::TextButton& t, bool shouldDrawHighlighted, bool shouldDrawDown) override;
private:
    juce::Font buttonFont;
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
    juce::Font getTextButtonFont(juce::TextButton &, int buttonHeight) override
    {
        return UXPalette::bebasNeue((float)buttonHeight);
    }
    void drawButtonBackground (juce::Graphics &g, juce::Button &b, const juce::Colour &backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        g.setColour(UXPalette::lightGray);
        if(b.getToggleState())
            g.setColour(UXPalette::lightRed);
        g.fillRoundedRectangle(b.getLocalBounds().toFloat(), 6.0f);
    }
    void drawButtonText(juce::Graphics& g, juce::TextButton& t, bool shouldDrawHighlighted, bool shouldDrawDown) override
    {
        auto fBounds = t.getLocalBounds().toFloat();
        auto str = t.getButtonText();
        auto delta = fBounds.getHeight() / 8.0f;
        fBounds = fBounds.reduced(delta);
        g.setColour(juce::Colours::white);
        g.setFont(getTextButtonFont(t, fBounds.getHeight()));
        g.drawText(str, fBounds, juce::Justification::centred);
    }
    
};

class TabbedButtonLnF : public juce::LookAndFeel_V4
{
public:
    TabbedButtonLnF()
    {
        setDefaultSansSerifTypefaceName("Bebas Neue");
    }
    juce::Font getTabButtonFont (juce::TabBarButton &, float height) override
    {
        return UXPalette::bebasNeue(height);
    }
    void createTabButtonShape(juce::TabBarButton& b, juce::Path& path, bool isMouseOver, bool isMouseDown) override
    {
        auto fBounds = b.getLocalBounds().toFloat();
        auto bottomLeft = std::make_pair(fBounds.getX(), fBounds.getBottom());
        auto bottomRight = std::make_pair(fBounds.getRight(), fBounds.getBottom());
        auto dX = fBounds.getWidth() * 0.15f;
        auto topLeft = std::make_pair(fBounds.getX() + dX, fBounds.getY());
        auto topRight = std::make_pair(fBounds.getRight() - dX, fBounds.getY());
        path.startNewSubPath(topLeft.first, topLeft.second);
        path.lineTo(topRight.first, topRight.second);
        path.lineTo(bottomRight.first, bottomRight.second);
        path.lineTo(bottomLeft.first, bottomLeft.second);
        path.closeSubPath();
    }
    void drawTabButton (juce::TabBarButton &b, juce::Graphics &g, bool isMouseOver, bool isMouseDown) override
    {
        juce::Path path;
        auto fBounds = b.getLocalBounds().toFloat();
        auto bottomLeft = std::make_pair(fBounds.getX(), fBounds.getBottom());
        auto bottomRight = std::make_pair(fBounds.getRight(), fBounds.getBottom());
        auto dX = fBounds.getWidth() * 0.15f;
        auto topLeft = std::make_pair(fBounds.getX() + dX, fBounds.getY());
        auto topRight = std::make_pair(fBounds.getRight() - dX, fBounds.getY());
        path.startNewSubPath(topLeft.first, topLeft.second);
        path.lineTo(topRight.first, topRight.second);
        path.lineTo(bottomRight.first, bottomRight.second);
        path.lineTo(bottomLeft.first, bottomLeft.second);
        path.closeSubPath();
        g.setColour(b.getTabBackgroundColour());
        if(b.isFrontTab())
            g.setColour(UXPalette::lightGray);
        g.fillPath(path);
        drawTabButtonText(b, g, isMouseOver, isMouseDown);
    }
    void drawTabButtonText(juce::TabBarButton &b, juce::Graphics &g, bool isMouseOver, bool isMouseDown) override
    {
        g.setFont(getTabButtonFont(b, (float)b.getTextArea().getHeight() * 0.75f));
        auto fBounds = b.getLocalBounds().toFloat();
        fBounds = fBounds.reduced(fBounds.getHeight() / 8.0f);
        auto& bar = b.getTabbedButtonBar();
        auto tabNames = bar.getTabNames();
        auto idx = bar.indexOfTabButton(&b);
        auto& str = tabNames[idx];
        g.setColour(juce::Colours::white);
        g.drawText(str, fBounds.toType<int>(), juce::Justification::centred);
    }
    
};
