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

namespace AlgorithmGridConstants
{
const int xCells = 3;
const int yCells = 3;
const float cellSideLength = 55.0f;
const float routingStrokeWidth = 0.75f;
static int topLeftX;
static int topLeftY;
static juce::Rectangle<float> graphBounds;
std::pair<float, float> getCellCenter(int x, int y)
{
    auto screenX = topLeftX + ((1 + x) * cellSideLength) - cellSideLength / 2.0f;
    auto screenY = topLeftY + ((1 + y) * cellSideLength) - cellSideLength / 2.0f;
    return std::pair<float, float>(screenX, screenY);
}
}

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


enum class RoutingDirection
{
    down,
    left,
    right,
    downLeft,
    downRight,
    selfMod
};

class RoutingPath
{
public:
    RoutingPath()
    {
    }
    void addPathAt(int gridX, int gridY, RoutingDirection dir);
    std::vector<juce::Path> paths;
};

class AlgorithmGraph : public juce::Component, public juce::Timer
{
public:
    AlgorithmGraph()
    {
        startTimerHz(24);
        pathColor = juce::Colours::black;
    }
    void timerCallback() override;
    void resized() override;
    void paint(juce::Graphics& g) override;
    juce::Colour pathColor;
    juce::Colour background;
};
