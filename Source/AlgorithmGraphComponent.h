/*
  ==============================================================================

    AlgorithmGraphComponent.h
    Created: 9 Apr 2021 11:32:49am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "RGBColor.h"
#include "ParameterStructure.h"

struct VectorUtil
{
    template <typename T>
    static void addIfUnique(std::vector<T>& list, T member)
    {
        bool inList = false;
        for(auto i : list)
        {
            if(i == member)
            {
                inList = true;
                break;
            }
        }
        if(!inList)
        {
            list.push_back(member);
        }
    }
    template <typename T>
    static bool contains(std::vector<T>& list, T member)
    {
        for(auto i : list)
        {
            if(member == i)
                return true;
        }
        return false;
    }
};

namespace AlgorithmGridConstants
{
static int unitWidth = 7;
static float cellSideLength = 55.0f;
static float routingStrokeWidth = 0.75f;
static int topLeftX;
static int topLeftY;
static juce::Rectangle<float> graphBounds;
}

struct OpInfo
{
    OpInfo(int idx) : index(idx)
    {
        gridX = 0;
        gridY = 0;
        modOrder = std::numeric_limits<int>::max();
    }
    std::vector<OpInfo*> sources;
    std::vector<OpInfo*> dests;
    int index;
    int gridX;
    int gridY;
    bool isActive = false;
    int modOrder = std::numeric_limits<int>::max();
    bool operator ==(const OpInfo test)
    {
        if(index == test.index)
            return true;
        return false;
    }
};

class OperatorBox : public juce::Component
{
public:
    OperatorBox(int gridX, int gridY, int idx) : xPos(gridX), yPos(gridY), index(idx)
    {
        sideLength = AlgorithmGridConstants::cellSideLength * 0.75f;
        fOffset = AlgorithmGridConstants::cellSideLength - sideLength;
        fillColor = Color::RGBColor(86, 110, 116);
        textColor = juce::Colours::black;
    }
    OperatorBox(int idx) : xPos(0), yPos(0), index(idx)
    {
        sideLength = AlgorithmGridConstants::cellSideLength * 0.75f;
        fOffset = AlgorithmGridConstants::cellSideLength - sideLength;
        fillColor = Color::RGBColor(86, 110, 116);
        textColor = juce::Colours::black;
    }
    ~OperatorBox() {}
    void resized() override;
    void paint(juce::Graphics& g) override;
    float sideLength;
    float fOffset;
    int xPos;
    int yPos;
    int index;
    juce::Rectangle<float> filledBounds;
    juce::Colour fillColor;
    juce::Colour textColor;
};

class AlgorithmGraph : public juce::Component, public juce::Timer
{
public:
    AlgorithmGraph()
    {
        setRepaintsOnMouseActivity(false);
        for(int i = 0; i < 6; ++i)
        {
            opInfo.add(new OpInfo(i));
        }
        startTimerHz(24);
        pathColor = juce::Colours::black;
    }
    
    void timerCallback() override;
    void resized() override;
    void updateOpInfo();
    int calculateRows(); //returns total number of rows;
    void addOperatorComponent(int gX, int gY, int idx)
    {
        opBoxes.add(new OperatorBox(gX, gY, idx));
        auto* op = opBoxes.getLast();
        addAndMakeVisible(op);
        auto dL = AlgorithmGridConstants::cellSideLength;
        op->setBounds(gX * dL, gY * dL, dL, dL);
        resized();
    }
    void paint(juce::Graphics& g) override;
    std::pair<float, float> getCellCenter(int x, int y)
    {
        auto screenX = AlgorithmGridConstants::topLeftX + ((1 + x) * AlgorithmGridConstants::cellSideLength) - AlgorithmGridConstants::cellSideLength / 2.0f;
        auto screenY = AlgorithmGridConstants::topLeftY + ((1 + y) * AlgorithmGridConstants::cellSideLength) - AlgorithmGridConstants::cellSideLength / 2.0f;
        return std::pair<float, float>(screenX, screenY);
    }
    void addPath(std::pair<int, int> from, std::pair<int, int>);
    juce::OwnedArray<juce::Path> paths;
    std::vector<OpInfo*> toDraw;
    std::vector<OpInfo*> bottomLevel;
    std::vector<OpInfo*> topLevel;
    std::vector<std::vector<OpInfo*>> gridRows;
    juce::OwnedArray<OpInfo> opInfo;
    juce::OwnedArray<OperatorBox> opBoxes;
    juce::Colour pathColor;
    juce::Colour background;
};
