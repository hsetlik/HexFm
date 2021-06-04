/*
  ==============================================================================

    WaveTypeSelector.cpp
    Created: 4 Jun 2021 8:44:02am
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "WaveTypeSelector.h"

ImageToggle::ImageToggle(juce::Image on, juce::Image off) : onImg(on), offImg(off)
{
    setClickingTogglesState(true);
}

void ImageToggle::paintButton(juce::Graphics &g, bool down, bool highlighted)
{
    auto image = (getToggleState()) ? onImg : offImg;
    auto fBounds = getLocalBounds();
    g.drawImageAt(image, fBounds.getX(), fBounds.getY());
}
//==============================================================================

WaveButtonSet::WaveButtonSet(int opIndex) :
sineButton(ImageToggle::getForWave(Sine, true), ImageToggle::getForWave(Sine, false)),
squareButton(ImageToggle::getForWave(Square, true), ImageToggle::getForWave(Square, false)),
sawButton(ImageToggle::getForWave(Saw, true), ImageToggle::getForWave(Saw, false)),
triButton(ImageToggle::getForWave(Tri, true), ImageToggle::getForWave(Tri, false))
{
    addAndMakeVisible(&sineButton);
    addAndMakeVisible(&squareButton);
    addAndMakeVisible(&sawButton);
    addAndMakeVisible(&triButton);
    int radioNum = 60 + opIndex;
    sineButton.setRadioGroupId(radioNum);
    squareButton.setRadioGroupId(radioNum);
    sawButton.setRadioGroupId(radioNum);
    triButton.setRadioGroupId(radioNum);
}

void WaveButtonSet::resized()
{
    auto bounds = getLocalBounds();
    auto dX = bounds.getWidth() / 4;
    sineButton.setBounds(bounds.removeFromLeft(dX));
    squareButton.setBounds(bounds.removeFromLeft(dX));
    sawButton.setBounds(bounds.removeFromLeft(dX));
    triButton.setBounds(bounds);
}

