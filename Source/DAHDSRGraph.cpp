/*
  ==============================================================================

    DAHDSRGraph.cpp
    Created: 8 Oct 2020 10:33:41am
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "DAHDSRGraph.h"

DAHDSRGraph::DAHDSRGraph(juce::Slider* Delay,
                         juce::Slider* Attack,
                         juce::Slider* Hold,
                         juce::Slider* Decay,
                         juce::Slider* Sustain,
                         juce::Slider* Release):
sDelay(Delay),
sAttack(Attack),
sHold(Hold),
sDecay(Decay),
sSustain(Sustain),
sRelease(Release)
{
    fDelay = 0.0f;
    fAttack = 20.0f;
    fHold = 0.0f;
    fDecay = 100.0f;
    fSustain = 0.6f;
    fRelease = 40.0f;
    bgColor = Color::RGBColor(37, 50, 53);
    traceColor = Color::RGBColor(250, 172, 63);
    startTimerHz(24);
}

void DAHDSRGraph::grabValues()
{
    if(sAttack->getValue() > 0.0f)
    {
        fDelay = sDelay->getValue();
        fAttack = sAttack->getValue();
        fHold = sHold->getValue();
        fDecay = sDecay->getValue();
        fSustain = sSustain->getValue();
        fRelease = sRelease->getValue();
    }
}

void DAHDSRGraph::paint(juce::Graphics &g)
{
    
    g.fillAll(bgColor);
    
    auto area = getLocalBounds().toFloat();
    auto timeTotal = fDelay + fAttack + fHold + fDecay + fRelease;
    juce::Path trace;
    trace.startNewSubPath(0.0f, area.getHeight());
    trace.lineTo(fDelay, area.getHeight());
    trace.lineTo(fDelay + fAttack, 0.0f);
    trace.lineTo(fDelay + fAttack + fHold, 0.0f);
    auto sustainY = (1.0f - fSustain) * area.getHeight();
    
    auto sustainLength = timeTotal * 0.25;
    trace.lineTo(fDelay + fAttack + fHold + fDecay, sustainY);
    trace.lineTo(fDelay + fAttack + fHold + fDecay + sustainLength, sustainY);
    trace.lineTo(timeTotal + sustainLength, area.getHeight());
    trace.closeSubPath();
    trace.scaleToFit(0.0f, 5.0f, area.getWidth(), (area.getHeight() - 5.0f), false);
    
    auto stroke = juce::PathStrokeType(1.0f);
    g.setColour(traceColor);
    g.strokePath(trace, stroke);
}
