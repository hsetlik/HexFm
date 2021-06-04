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

WaveButtonSet::WaveButtonSet(int opIndex, juce::AudioProcessorValueTreeState* tree) :
currentType(Sine),
sineButton(ImageToggle::getForWave(Sine, true), ImageToggle::getForWave(Sine, false)),
squareButton(ImageToggle::getForWave(Square, true), ImageToggle::getForWave(Square, false)),
sawButton(ImageToggle::getForWave(Saw, true), ImageToggle::getForWave(Saw, false)),
triButton(ImageToggle::getForWave(Tri, true), ImageToggle::getForWave(Tri, false))
{
    addAndMakeVisible(&sineButton);
    addAndMakeVisible(&squareButton);
    addAndMakeVisible(&sawButton);
    addAndMakeVisible(&triButton);
    sineButton.addListener(this);
    squareButton.addListener(this);
    sawButton.addListener(this);
    triButton.addListener(this);
    
    addAndMakeVisible(&hiddenBox);
    hiddenBox.addListener(this);
    hiddenBox.addItem("Sine", 1);
    hiddenBox.addItem("Square", 2);
    hiddenBox.addItem("Saw", 3);
    hiddenBox.addItem("Tri", 4);
    
    int radioNum = 60 + opIndex;
    sineButton.setRadioGroupId(radioNum);
    squareButton.setRadioGroupId(radioNum);
    sawButton.setRadioGroupId(radioNum);
    triButton.setRadioGroupId(radioNum);
    auto waveId = "waveParam" + juce::String(opIndex);
    waveAttach.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(*tree, waveId, hiddenBox));
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

void WaveButtonSet::buttonClicked(juce::Button *b)
{
    if(b == &sineButton)
    {
        currentType = Sine;
        hiddenBox.setSelectedId(1);
    }
    else if(b == &squareButton)
    {
        currentType = Square;
        hiddenBox.setSelectedId(2);
    }
    else if(b == &sawButton)
    {
        currentType = Saw;
        hiddenBox.setSelectedId(3);
    }
    else if(b == &triButton)
    {
        currentType = Tri;
        hiddenBox.setSelectedId(4);
    }
}

void WaveButtonSet::comboBoxChanged(juce::ComboBox *box)
{
    auto waveId = box->getSelectedId();
    if(waveId == 1 && !sineButton.getToggleState())
        sineButton.triggerClick();
    else if(waveId == 2 && !squareButton.getToggleState())
        squareButton.triggerClick();
    else if(waveId == 3 && !sawButton.getToggleState())
        sawButton.triggerClick();
    else if(waveId == 4 && !triButton.getToggleState())
        triButton.triggerClick();
}
