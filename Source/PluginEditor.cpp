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
    operatorColor = Color::RGBColor(51, 81, 90);
    for(int i = 0; i < numOperators; ++i)
    {
        allOps.add(new OperatorComponent(i, &audioProcessor.tree));
        addAndMakeVisible(*allOps.getLast());
    }
    
    addAndMakeVisible(&algGraph);
    addAndMakeVisible(&patchLoader);
    
    patchLoader.patchSelector.setSelectedItemIndex(1);
    addAndMakeVisible(&saveDialog);
    saveDialog.setEnabled(false);
    
    addAndMakeVisible(&modGrid);
    modGrid.attachButtons(&audioProcessor.tree);
    addAndMakeVisible(&lfoGroup);
    lfoGroup.attachChildren(&audioProcessor.tree);
    setSize (1000, 800);
}

HexFmAudioProcessorEditor::~HexFmAudioProcessorEditor()
{
}
//==============================================================================
void HexFmAudioProcessorEditor::paint(juce::Graphics &g)
{
    auto background = Color::RGBColor(37, 50, 53);
    g.fillAll(background);
    for(int i = 0; i < 6; ++i)
    {
        auto rect = allOps[i]->getBounds();
        g.setColour(UXPalette::darkGray1);
        g.fillRect(rect);
        auto centerRect = rect.reduced(2);
        g.setColour(UXPalette::darkGray3);
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
    //w = 250
    int h = getHeight() / 2;
    saveDialog.setBounds(w, h / 2, 2 * w, h);
     //Screen dimensions are set to: 1000, 800
     //overall aspect: 5/4
    dY = getHeight() / 10;
    allOps[0]->setBounds(0, dY, 3 * dY, 4.5 * dY);
    allOps[1]->setBounds(3 * dY, dY, 3 * dY, 4.5 * dY);
    allOps[2]->setBounds(6 * dY, dY, 3 * dY, 4.5 * dY);
    allOps[3]->setBounds(0, 5.5 * dY, 3 * dY, 4.5 * dY);
    allOps[4]->setBounds(3 * dY, 5.5 * dY, 3 * dY, 4.5 * dY);
    allOps[5]->setBounds(6 * dY, 5.5 * dY, 3 * dY, 4.5 * dY);
    
    modGrid.setBounds(9 * dY, 4 * dY, 3 * dY, 3 * dY);
    lfoGroup.setBounds(9 * dY, 7 * dY, 3 * dY, 3 * dY);
    algGraph.setBounds(9 * dY, dY, 3 * dY, 3 * dY);
    
    patchLoader.setBounds(3 * dY, 0, 9 * dY, dY);
}


