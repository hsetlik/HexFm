/*
  ==============================================================================

    OperatorComponent.h
    Created: 6 Oct 2020 1:08:26pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SliderSubclasses.h"
#include "DAHDSRGraph.h"
#include "RGBColor.h"
#include "WaveTypeSelector.h"

//==============================================================================
/*
*/
class OperatorComponent  : public juce::Component, public juce::Button::Listener
{
public:
    OperatorComponent(int index, juce::AudioProcessorValueTreeState* pTree);
    ~OperatorComponent() override;
    void attachAll(juce::AudioProcessorValueTreeState* pTree)
    {
        outputButton.attach(pTree);
        levelSlider.attach(pTree);
        ratioSlider.attach(pTree);
        modIndexSlider.attach(pTree);
        panSlider.attach(pTree);
        
        delaySlider.attach(pTree);
        attackSlider.attach(pTree);
        holdSlider.attach(pTree);
        decaySlider.attach(pTree);
        sustainSlider.attach(pTree);
        releaseSlider.attach(pTree);
    }
    void buttonClicked(juce::Button* b) override;
    void resized() override;
    void paint(juce::Graphics& g) override;
    int opIndex;
private:
    DAHDSRGraph envGraph;
    WaveButtonSet waveButtons;
    juce::Colour background;
    LevelSlider levelSlider;
    RatioSlider ratioSlider;
    EnvLabel ratioLabel;
    ModIndexSlider modIndexSlider;
    EnvLabel indexLabel;
    OutputToggleButton outputButton;
    
    DelaySlider delaySlider;
    EnvLabel delayLabel;
    AttackSlider attackSlider;
    EnvLabel attackLabel;
    HoldSlider holdSlider;
    EnvLabel holdLabel;
    DecaySlider decaySlider;
    EnvLabel decayLabel;
    SustainSlider sustainSlider;
    EnvLabel sustainLabel;
    ReleaseSlider releaseSlider;
    EnvLabel releaseLabel;
    
    PanSlider panSlider;
    EnvLabel panLabel;
    
    LnF1 look1;
    LnF2 look2;
    OutputButtonLnF outputLnF;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OperatorComponent)
};
