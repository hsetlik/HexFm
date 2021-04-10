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
};

namespace AlgorithmGridConstants
{
const int xCells = 7;
static int yCells = 7;
static float cellSideLength = 55.0f;
static float routingStrokeWidth = 0.75f;
static int topLeftX;
static int topLeftY;
static juce::Rectangle<float> graphBounds;
}

struct OperatorInfo
{
    OperatorInfo()
    {
        sources.clear();
        sources.reserve(sizeof(OperatorInfo*) * 6);
        dests.clear();
        dests.reserve(sizeof(OperatorInfo*) * 6);
        gridX = 0;
        gridY = 0;
        modOrder = std::numeric_limits<int>::max();
    }
    std::vector<OperatorInfo*> sources;
    std::vector<OperatorInfo*> dests;
    int index = 0;
    int gridX = 0;
    int gridY = 0;
    int modOrder = std::numeric_limits<int>::max();
    void init()
    {
        sources.clear();
        sources.reserve(6);
        dests.clear();
        dests.reserve(6);
        gridX = 0;
        gridY = 0;
        modOrder = std::numeric_limits<int>::max();
    }
    void setModOrder() //call this on each bottom-level operator to recursively set up all mod orders
    {
        if(dests.capacity() < 1)
            dests.reserve(6);
        int highestOrder = std::numeric_limits<int>::max();
        if(dests.size() > 0)
        {
            auto& check = dests;
            for(int i = 0; i < (int)dests.size(); ++i)
            {
                auto op = dests[i];
                if(op->modOrder < highestOrder)
                {
                    highestOrder = op->modOrder;
                }
            }
        }
        modOrder = highestOrder + 1;
        for(auto op : sources)
            op->setModOrder();
    }
    bool operator ==(OperatorInfo test)
    {
        if(this == &test)
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


using OpVector = std::vector<OperatorInfo*>;
class AlgorithmGraph : public juce::Component, public juce::Timer
{
public:
    AlgorithmGraph()
    {
        
        startTimerHz(24);
        pathColor = juce::Colours::black;
        for(int i = 0; i < 6; ++i)
            opInfo[i].index = i;
    }
    void timerCallback() override;
    void resized() override;
    void updateOpInfo();
    void calculateRows();
    void paint(juce::Graphics& g) override;
    std::pair<float, float> getCellCenter(int x, int y)
    {
        auto screenX = AlgorithmGridConstants::topLeftX + ((1 + x) * AlgorithmGridConstants::cellSideLength) - AlgorithmGridConstants::cellSideLength / 2.0f;
        auto screenY = AlgorithmGridConstants::topLeftY + ((1 + y) * AlgorithmGridConstants::cellSideLength) - AlgorithmGridConstants::cellSideLength / 2.0f;
        return std::pair<float, float>(screenX, screenY);
    }
    void addPath(std::pair<int, int> from, std::pair<int, int>);
    void initOperators()
    {
        int idx = 0;
        for(auto op : opInfo)
        {
            op.init();
            op.index = idx;
            ++idx;
        }
    }
    std::vector<juce::Path> paths;
    std::array<OperatorInfo, 6> opInfo;
    OpVector toDraw;
    OpVector bottomLevel;
    OpVector gridRows;
    juce::Colour pathColor;
    juce::Colour background;
};
