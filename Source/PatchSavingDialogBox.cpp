/*
  ==============================================================================

    PatchSavingDialogBox.cpp
    Created: 12 Oct 2020 11:01:32am
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "PatchSavingDialogBox.h"

PatchDialogBox::PatchDialogBox(PatchLoader* loader) : patchLoader(loader)
{
    savePatchButton.addListener(this);
    cancelButton.addListener(this);
    addAndMakeVisible(&nameField);
    nameField.setMultiLine(false);
    nameField.setTextToShowWhenEmpty("Patch Name", UXPalette::lightGray);
    nameField.setPopupMenuEnabled(false);
    
    addAndMakeVisible(&savePatchButton);
    savePatchButton.setButtonText("Save patch");
    
    addAndMakeVisible(&cancelButton);
    cancelButton.setButtonText("Cancel");
    
    addAndMakeVisible(&typeBox);
    int idx = 1;
    for(auto t : PatchTypeStrings)
    {
        typeBox.addItem(t, idx);
        ++idx;
    }
    typeBox.setSelectedId(1);
}

void PatchDialogBox::paint(juce::Graphics &g)
{
    g.fillAll(UXPalette::nearBlack);
    auto cushion = getHeight() / 14.0f;
    auto lightArea = getLocalBounds().toFloat().reduced(cushion);
    g.setColour(UXPalette::darkGray1);
    g.fillRect(lightArea);
    auto labelFont = UXPalette::square(12.0f);
    g.setFont(labelFont);
    g.setColour(juce::Colours::white);
    int w = getWidth() / 18;
    int h = getHeight() / 8;
    auto textBox = juce::Rectangle<int> {w, h, 8 * w, 2 * h};
    g.drawText("Patch Name:", textBox, juce::Justification::left);
}


void PatchDialogBox::resized()
{
    int w = getWidth() / 18;
    int h = getHeight() / 12;
    
    nameField.setBounds(w, 3.5 * h, 16 * w, h);
    savePatchButton.setBounds(9 * w, 5 * h, 5 * w, 1.5 * h);
    cancelButton.setBounds(15 * w, 5 * h, 2 * w, 1.5 * h);
    typeBox.setBounds(w, 5 * h, 3 * w, 1.5 * h);
    auto name = patchLoader->getCurrentPresetName();
    auto type = patchLoader->getCurrentPresetType();
    nameField.setText(name);
    for(int i = 0; i < typeBox.getNumItems(); ++i)
    {
        if(typeBox.getItemText(i) == type)
            typeBox.setSelectedItemIndex(i);
    }
}

void PatchDialogBox::buttonClicked(juce::Button *button)
{
    if(button == &savePatchButton)
    {
        auto patchName = nameField.getText();
        patchLoader->savePreset(patchName, typeBox.getText());
    }
        nameField.clear();
        setEnabled(false);
        setVisible(false);
}
