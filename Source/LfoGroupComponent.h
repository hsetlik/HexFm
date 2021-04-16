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
#include "FilterPanel.h"
#include "WaveGraph.h"

class LfoGroupComponent : public juce::TabbedComponent
{
public:
    LfoGroupComponent() : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
    {
        auto tabColor = UXPalette::darkGray3;
        setTabBarDepth(25);
        for(int i = 0; i < 4; ++i)
        {
            children.add(new LfoComponent(i));
            auto tabName = "LFO " + juce::String(i + 1);
            addTab(tabName, tabColor, children.getLast(), false);
        }
        addTab("Filter", tabColor, &fPanel, false);
    }
    ~LfoGroupComponent() {}
    void attachChildren(juce::AudioProcessorValueTreeState* pTree)
    {
        for(int i = 0; i < children.size(); ++i)
        {
            children[i]->attachAll(pTree);
        }
        fPanel.attach(pTree);
    }
private:
    juce::OwnedArray<LfoComponent> children;
    FilterPanel fPanel;
};
