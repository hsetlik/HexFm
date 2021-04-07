/*
  ==============================================================================

    LfoComponent.cpp
    Created: 9 Oct 2020 9:25:18am
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "LfoComponent.h"

LfoModeSelector::LfoModeSelector(int index) : modeSelectorIndex(index)
{
    upOffImg = juce::ImageCache::getFromMemory(BinaryData::upOff_png, BinaryData::upOff_pngSize);
    upOnImg = juce::ImageCache::getFromMemory(BinaryData::upOn_png, BinaryData::upOn_pngSize);
    bothOffImg = juce::ImageCache::getFromMemory(BinaryData::bothOff_png, BinaryData::bothOff_pngSize);
    bothOnImg = juce::ImageCache::getFromMemory(BinaryData::bothOn_png, BinaryData::bothOn_pngSize);
    downOffImg = juce::ImageCache::getFromMemory(BinaryData::downOff_png, BinaryData::downOff_pngSize);
    downOnImg = juce::ImageCache::getFromMemory(BinaryData::downOn_png, BinaryData::downOn_pngSize);
    
    addAndMakeVisible(upButton);
    addAndMakeVisible(bothButton);
    addAndMakeVisible(downButton);
    addAndMakeVisible(choiceHandler);
    choiceHandler.setVisible(false);
    auto none = juce::Colours::transparentBlack;
    upButton.setImages(true, true, true, upOffImg, 1.0f, none, upOffImg, 1.0f, none, upOnImg, 1.0f, none);
    upButton.setClickingTogglesState(true);
    bothButton.setImages(true, true, true, bothOffImg, 1.0f, none, bothOffImg, 1.0f, none, bothOnImg, 1.0f, none);
    bothButton.setClickingTogglesState(true);
    downButton.setImages(true, true, true, downOffImg, 1.0f, none, downOffImg, 1.0f, none, downOnImg, 1.0f, none);
    downButton.setClickingTogglesState(true);
    
    upButton.setRadioGroupId(index + 1);
    bothButton.setRadioGroupId(index + 1);
    downButton.setRadioGroupId(index + 1);
    
    choiceHandler.addItem("up", 1);
    choiceHandler.addItem("both", 2);
    choiceHandler.addItem("down", 3);
    
    upButton.setTooltip("Modulate ratio upwards");
    bothButton.setTooltip("Modulate ratio up and down");
    downButton.setTooltip("Modulate ratio downwards");
    upButton.triggerClick();
}

void LfoModeSelector::buttonClicked(juce::Button *b)
{
    if(b == &upButton)
        choiceHandler.setSelectedId(1);
    else if(b == &bothButton)
        choiceHandler.setSelectedId(2);
    else
        choiceHandler.setSelectedId(3);
}

void LfoModeSelector::resized()
{
    auto bWidth = getWidth() / 3;
    upButton.setBounds(0, 0, bWidth, getHeight());
    bothButton.setBounds(bWidth, 0, bWidth, getHeight());
    downButton.setBounds(2 * bWidth, 0, bWidth, getHeight());
}

void LfoModeSelector::attach(juce::AudioProcessorValueTreeState *tree)
{
    auto nStr = juce::String(modeSelectorIndex);
    auto modeId = "lfoRatioModeParam" + nStr;
    modeAttach.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(*tree, modeId, choiceHandler));
    
}

LfoComponent::LfoComponent(int index) : lfoIndex(index), selector(index)
{
    addAndMakeVisible(&rateSlider);
    rateSlider.setRange(0.0f, 20.0f);
    rateSlider.setSliderStyle(juce::Slider::Rotary);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 45, 15);
    
    addAndMakeVisible(&levelSlider);
    levelSlider.setRange(0.0f, 1.0f);
    levelSlider.setSliderStyle(juce::Slider::Rotary);
    levelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 45, 15);
    
    addAndMakeVisible(&waveSelector);
    addAndMakeVisible(&targetSelector);
    addAndMakeVisible(&selector);
    selector.setVisible(false);
    selector.setEnabled(false);
    targetSelector.addItem("No target", 1);
    int numTargets = 1;
    for(int i = 0; i < 6; ++i)
    {
        auto iStr = juce::String(i + 1);
        auto opLevel = "Operator " + iStr + " level";
        auto opRatio = "Operator " + iStr + " ratio";
        targetSelector.addItem(opLevel, numTargets + 1);
        ++numTargets;
        targetSelector.addItem(opRatio, numTargets + 1);
        ++numTargets;
    }
    targetSelector.setTooltip("LFO target");
    targetSelector.addListener(this);
    
    waveSelector.addItem("Sine", 1);
    waveSelector.addItem("Triangle", 2);
    waveSelector.addItem("Saw", 3);
    waveSelector.addItem("Square", 4);
    waveSelector.addItem("Random", 5);
    
    waveSelector.setTooltip("LFO wave shape");
}

void LfoComponent::comboBoxChanged(juce::ComboBox *c)
{
    auto index = c->getSelectedItemIndex();
    if(index % 2 != 0 || index == 0)
    {
        selector.setVisible(false);
        selector.setEnabled(false);
    }
    else
    {
        selector.setVisible(true);
        selector.setEnabled(true);
        selector.initialize();
    }
}

void LfoComponent::attachAll(juce::AudioProcessorValueTreeState *pTree)
{
    auto iStr = juce::String(lfoIndex);
    auto rateId = "lfoRateParam" + iStr;
    rateAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*pTree, rateId, rateSlider));
    
    auto levelId = "lfoLevelParam" + iStr;
    levelAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*pTree, levelId, levelSlider));
    
    auto waveId = "lfoWaveParam" + iStr;
    waveTypeAttach.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(*pTree, waveId, waveSelector));
    
    auto targetId = "lfoTargetParam" + iStr;
    targetAttach.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(*pTree, targetId, targetSelector));
    
    selector.attach(pTree);
}

void LfoComponent::resized()
{
    int n = getWidth() / 24;
    levelSlider.setBounds(2 * n, 2 * n, 5 * n, 7 * n);
    rateSlider.setBounds(2 * n, 13 * n, 5 * n, 7 * n);
    
    selector.setBounds(8 * n, 8 * n, 97, 33);
    
    waveSelector.setBounds(7 * n, 4.5 * n, 10 * n, 2 * n);
    targetSelector.setBounds(8 * n, 12 * n, 14 * n, 2 * n);
}

void LfoComponent::paint(juce::Graphics &g)
{
    auto n = getWidth() / 24;
    auto lLabelBox = juce::Rectangle<int> {2 * n, int(0.5 * n), 5 * n, 2 * n};
    auto rLabelBox = juce::Rectangle<int> {2 * n, int(12 * n), 5 * n, 2 * n};
    g.setFont(juce::Font("VCR OSD Mono", 13.0f, 0));
    g.setColour(juce::Colours::white);
    g.drawText("Level", lLabelBox, juce::Justification::centredBottom);
    g.drawText("Rate", rLabelBox, juce::Justification::centredBottom);
}
