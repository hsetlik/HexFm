/*
  ==============================================================================

    LfoGroupComponent.h
    Created: 9 Oct 2020 9:25:37am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LfoComponent.h"
#include "WaveGraph.h"

class LfoGroupComponent : public juce::TabbedComponent
{
public:
    LfoGroupComponent() : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
    {
        auto tabColor = ColorCreator::RGBColor(51, 81, 90);
        setTabBarDepth(25);
        for(int i = 0; i < 4; ++i)
        {
            children.add(new LfoComponent(i));
            auto tabName = "LFO " + juce::String(i + 1);
            addTab(tabName, tabColor, children.getLast(), false);
        }
    }
    ~LfoGroupComponent() {}
    void attachChildren(juce::AudioProcessorValueTreeState* pTree)
    {
        for(int i = 0; i < children.size(); ++i)
        {
            children[i]->attachAll(pTree);
        }
    }
private:
    juce::OwnedArray<LfoComponent> children;
};
