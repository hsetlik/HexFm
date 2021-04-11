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
releaseLabel(&releaseSlider, "ms"),
panSlider(index),
panLabel(&panSlider, "")
{
    attachAll(pTree);
    addAndMakeVisible(&levelSlider);
    levelSlider.setLookAndFeel(&look); //override the vertical bar draw method next
    addAndMakeVisible(&ratioSlider);
    ratioSlider.setLookAndFeel(&look);
    addAndMakeVisible(&modIndexSlider);
    modIndexSlider.slider.setLookAndFeel(&look);
    addAndMakeVisible(&outputButton);
    outputButton.addListener(this);
    addAndMakeVisible(&delaySlider);
    delaySlider.setLookAndFeel(&look);
    addAndMakeVisible(&attackSlider);
    attackSlider.setLookAndFeel(&look);
    addAndMakeVisible(&holdSlider);
    holdSlider.setLookAndFeel(&look);
    addAndMakeVisible(&decaySlider);
    decaySlider.setLookAndFeel(&look);
    addAndMakeVisible(&sustainSlider);
    sustainSlider.setLookAndFeel(&look);
    addAndMakeVisible(&releaseSlider);
    releaseSlider.setLookAndFeel(&look);
    addAndMakeVisible(&panSlider);
    panSlider.setLookAndFeel(&look);
    
    addAndMakeVisible(&ratioLabel);
    addAndMakeVisible(&indexLabel);
    addAndMakeVisible(&panLabel);
    
    addAndMakeVisible(&envGraph);
    
    addAndMakeVisible(&delayLabel);
    addAndMakeVisible(&attackLabel);
    addAndMakeVisible(&holdLabel);
    addAndMakeVisible(&decayLabel);
    addAndMakeVisible(&sustainLabel);
    addAndMakeVisible(&releaseLabel);
    
    panSlider.setVisible(false);
    panSlider.setEnabled(false);
    panLabel.setVisible(false);
    panLabel.setEnabled(false);
    
    outputButton.triggerClick();
    outputButton.triggerClick();
}

OperatorComponent::~OperatorComponent()
{
    levelSlider.setLookAndFeel(nullptr);
    ratioSlider.setLookAndFeel(nullptr);
    modIndexSlider.slider.setLookAndFeel(nullptr);
    delaySlider.setLookAndFeel(nullptr);
    attackSlider.setLookAndFeel(nullptr);
    holdSlider.setLookAndFeel(nullptr);
    decaySlider.setLookAndFeel(nullptr);
    sustainSlider.setLookAndFeel(nullptr);
    releaseSlider.setLookAndFeel(nullptr);
    panSlider.setLookAndFeel(nullptr);
}

void OperatorComponent::buttonClicked(juce::Button *b)
{
    bool panActive = b->getToggleState();
    if(panActive)
    {
        panSlider.setVisible(true);
        panSlider.setEnabled(true);
        panLabel.setVisible(true);
        panLabel.setEnabled(true);
    }
    else
    {
        panSlider.setVisible(false);
        panSlider.setEnabled(false);
        panLabel.setVisible(false);
        panLabel.setEnabled(false);
    }
    repaint();
}

void OperatorComponent::resized()
{
    int n = getWidth() / 24;
    auto fBounds = getBounds().toFloat().reduced(3.0f);
    auto dN = fBounds.getWidth() / 48.0f;
    delaySlider.setBounds(dN / 2, 50 * dN, 7.5 * dN, 7.5 * dN);
    attackSlider.setBounds(8.5 * dN, 50 * dN, 7.5 * dN, 7.5 * dN);
    holdSlider.setBounds(17 * dN, 50 * dN, 7.5 * dN, 7.5 * dN);
    decaySlider.setBounds(25 * dN, 50 * dN, 7.5 * dN, 7.5 * dN);
    sustainSlider.setBounds(33 * dN, 50 * dN, 7.5 * dN, 7.5 * dN);
    releaseSlider.setBounds(41 * dN, 50 * dN, 7.5 * dN, 7.5 * dN);
    
    delayLabel.setBounds(dN / 2, 56 * dN, 7.5 * dN, 3 * dN);
    attackLabel.setBounds(8.5 * dN, 56 * dN, 7.5 * dN, 3 * dN);
    holdLabel.setBounds(16.5 * dN, 56 * dN, 7.5 * dN, 3 * dN);
    decayLabel.setBounds(24.5 * dN, 56 * dN, 7.5 * dN, 3 * dN);
    sustainLabel.setBounds(32.5 * dN, 56 * dN, 7.5 * dN, 3 * dN);
    releaseLabel.setBounds(40.5 * dN, 56 * dN, 7.5 * dN, 3 * dN);
    
    envGraph.setBounds(n, 13 * n, 15 * n, 10 * n);

    outputButton.setBounds(16 * n, 1.5 * n, 5.5 * n, 2.5 * n);
    levelSlider.setBounds(19 * n, 13 * n, 4 * n, 10 * n);
    
    
    modIndexSlider.setBounds(5 * n, 6 * n, 5 * n, 5 * n);
    indexLabel.setBounds(4.5 * n, 11 * n, 6 * n, 2 * n);
    
    ratioSlider.setBounds(12 * n, 6 * n, 5 * n, 5 * n);
    ratioLabel.setBounds(11.5 * n, 11 * n, 6 * n, 2 * n);
    
    panSlider.setBounds(18 * n, 6 * n, 5 * n, 5 * n);
    panLabel.setBounds(17.5 * n, 11 * n, 6 * n, 2 * n);
}

void OperatorComponent::paint(juce::Graphics &g)
{
    int n = getWidth() / 24;
    auto iLabelBox = juce::Rectangle<int>{int(4.5 * n), int(4.5 * n), 6 * n, 2 * n};
    auto rLabelBox = juce::Rectangle<int>{int(11.5 * n), int(4.5 * n), 6 * n, 2 * n};
    auto lLabelBox = juce::Rectangle<int>{19 * n, 13 * n, 4 * n, 2 * n};
    auto pLabelBox = juce::Rectangle<int>{19 * n, int(4.5 * n), 4 * n, 2 * n};
    g.setFont(juce::Font("VCR OSD Mono", 15.0f, 0));
    g.setColour(juce::Colours::white);
    //g.fillRect(indexLabel.getBounds());
    auto nameRect = juce::Rectangle<int> {n * 3, n / 6, 10 * n, 6 * n};
    auto labelText = "Operator " + juce::String(opIndex + 1);
    g.drawText(labelText, nameRect, juce::Justification::centred);
    auto iText = "INDEX";
    auto rText = "RATIO";
    auto lText = "LEVEL";
    auto pText = "PAN";
    g.setFont(juce::Font("LCD", 11.0f, 0));
    g.drawText(iText, iLabelBox, juce::Justification::centredBottom);
    g.drawText(rText, rLabelBox, juce::Justification::centredBottom);
    g.drawText(lText, lLabelBox, juce::Justification::centredBottom);
    if(outputButton.getToggleState())
        g.drawText(pText, pLabelBox, juce::Justification::centredBottom);
    
}
