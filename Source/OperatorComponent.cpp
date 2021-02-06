/*
  ==============================================================================

    OperatorComponent.cpp
    Created: 6 Oct 2020 1:08:26pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OperatorComponent.h"

//==============================================================================
OperatorComponent::OperatorComponent(int index, juce::AudioProcessorValueTreeState* pTree) :
opIndex(index),
envGraph(&delaySlider, &attackSlider, &holdSlider, &decaySlider, &sustainSlider, &releaseSlider),
levelSlider(index),
ratioSlider(index),
ratioLabel(&ratioSlider.slider, ""),
modIndexSlider(index),
indexLabel(&modIndexSlider.slider, ""),
outputButton(index),
delaySlider(index),
delayLabel(&delaySlider, "ms"),
attackSlider(index),
attackLabel(&attackSlider, "ms"),
holdSlider(index),
holdLabel(&holdSlider, "ms"),
decaySlider(index),
decayLabel(&decaySlider, "ms"),
sustainSlider(index),
sustainLabel(&sustainSlider, ""),
releaseSlider(index),
releaseLabel(&releaseSlider, "ms")
{
    attachAll(pTree);
    addAndMakeVisible(&levelSlider);
    addAndMakeVisible(&ratioSlider);
    addAndMakeVisible(&modIndexSlider);
    addAndMakeVisible(&outputButton);
    addAndMakeVisible(&delaySlider);
    addAndMakeVisible(&attackSlider);
    addAndMakeVisible(&holdSlider);
    addAndMakeVisible(&decaySlider);
    addAndMakeVisible(&sustainSlider);
    addAndMakeVisible(&releaseSlider);
    
    addAndMakeVisible(&ratioLabel);
    addAndMakeVisible(&indexLabel);
    
    addAndMakeVisible(&envGraph);
    
    addAndMakeVisible(&delayLabel);
    addAndMakeVisible(&attackLabel);
    addAndMakeVisible(&holdLabel);
    addAndMakeVisible(&decayLabel);
    addAndMakeVisible(&sustainLabel);
    addAndMakeVisible(&releaseLabel);
}

OperatorComponent::~OperatorComponent()
{
}

void OperatorComponent::resized()
{
    int n = getWidth() / 24;
    delaySlider.setBounds(n / 2, 23 * n, 4 * n, 4 * n);
    attackSlider.setBounds(4.5 * n, 23 * n, 4 * n, 4 * n);
    holdSlider.setBounds(8.5 * n, 23 * n, 4 * n, 4 * n);
    decaySlider.setBounds(12.5 * n, 23 * n, 4 * n, 4 * n);
    sustainSlider.setBounds(16.5 * n, 23 * n, 4 * n, 4 * n);
    releaseSlider.setBounds(20.5 * n, 23 * n, 4 * n, 4 * n);
    
    delayLabel.setBounds(n / 2, 27 * n, 4 * n, 2 * n);
    attackLabel.setBounds(4.5 * n, 27 * n, 4 * n, 2 * n);
    holdLabel.setBounds(8.5 * n, 27 * n, 4 * n, 2 * n);
    decayLabel.setBounds(12.5 * n, 27 * n, 4 * n, 2 * n);
    sustainLabel.setBounds(16.5 * n, 27 * n, 4 * n, 2 * n);
    releaseLabel.setBounds(20.5 * n, 27 * n, 4 * n, 2 * n);
    
    envGraph.setBounds(n, 13 * n, 15 * n, 10 * n);

    outputButton.setBounds(16 * n, 1.5 * n, 5.5 * n, 2.5 * n);
    levelSlider.setBounds(19 * n, 6 * n, 4 * n, 12 * n);
    
    
    modIndexSlider.setBounds(5 * n, 6 * n, 5 * n, 5 * n);
    indexLabel.setBounds(4.5 * n, 11 * n, 6 * n, 2 * n);
    
    ratioSlider.setBounds(12 * n, 6 * n, 5 * n, 5 * n);
    ratioLabel.setBounds(11.5 * n, 11 * n, 6 * n, 2 * n);
}

void OperatorComponent::paint(juce::Graphics &g)
{
    int n = getWidth() / 24;
    auto iLabelBox = juce::Rectangle<int>{int(4.5 * n), int(4.5 * n), 6 * n, 2 * n};
    auto rLabelBox = juce::Rectangle<int>{int(11.5 * n), int(4.5 * n), 6 * n, 2 * n};
    g.setFont(juce::Font("VCR OSD Mono", 15.0f, 0));
    g.setColour(juce::Colours::white);
    //g.fillRect(indexLabel.getBounds());
    auto nameRect = juce::Rectangle<int> {n * 3, n / 6, 10 * n, 6 * n};
    auto labelText = "Operator " + juce::String(opIndex + 1);
    g.drawText(labelText, nameRect, juce::Justification::centred);
    auto iText = "INDEX";
    auto rText = "RATIO";
    g.setFont(juce::Font("LCD", 11.0f, 0));
    g.drawText(iText, iLabelBox, juce::Justification::centredBottom);
    g.drawText(rText, rLabelBox, juce::Justification::centredBottom);
}
