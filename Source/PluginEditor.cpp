/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HexFmAudioProcessorEditor::HexFmAudioProcessorEditor (HexFmAudioProcessor& p) :
AudioProcessorEditor (&p),
modGrid(numOperators),
patchLoader(&p, &saveDialog),
saveDialog(&patchLoader),
audioProcessor (p)
{
    operatorColor = ColorCreator::RGBColor(51, 81, 90);
    for(int i = 0; i < numOperators; ++i)
    {
        allOps.add(new OperatorComponent(i, &audioProcessor.tree));
        addAndMakeVisible(*allOps.getLast());
    }
    
    addAndMakeVisible(&patchLoader);
    
    patchLoader.patchSelector.setSelectedItemIndex(1);
    addAndMakeVisible(&saveDialog);
    saveDialog.setEnabled(false);
    
    addAndMakeVisible(&modGrid);
    modGrid.attachButtons(&audioProcessor.tree);
    addAndMakeVisible(&lfoGroup);
    lfoGroup.attachChildren(&audioProcessor.tree);
    setSize (1000, 600);
}

HexFmAudioProcessorEditor::~HexFmAudioProcessorEditor()
{
}
//==============================================================================
void HexFmAudioProcessorEditor::paint(juce::Graphics &g)
{
    auto background = ColorCreator::RGBColor(37, 50, 53);
    g.fillAll(background);
    for(int i = 0; i < 6; ++i)
    {
        auto rect = allOps[i]->getBounds();
        g.setColour(ColorCreator::blend(operatorColor, juce::Colours::black, 0.2));
        g.fillRect(rect);
        auto centerRect = rect.reduced(5);
        g.setColour(operatorColor);
        g.fillRect(centerRect);
    }
    if(!saveDialog.isEnabled())
    {
        saveDialog.setVisible(false);
        saveDialog.toBack();
    }
}

void HexFmAudioProcessorEditor::resized()
{
    int w = getWidth() / 4;
    int h = getHeight() / 2;
    allOps[0]->setBounds(0, 0, w, h);
    allOps[1]->setBounds(w, 0, w, h);
    allOps[2]->setBounds(2 * w, 0, w, h);
    allOps[3]->setBounds(0, h, w, h);
    allOps[4]->setBounds(w, h, w, h);
    allOps[5]->setBounds(2 * w, h, w, h);
    modGrid.setBounds(3 * w, 0, w, w);
    lfoGroup.setBounds(3 * w, w, w, w);
    patchLoader.setBounds(3 * w, 2 * w, w, getHeight() - (2 * w));
    saveDialog.setBounds(w, h / 2, 2 * w, h);
}
