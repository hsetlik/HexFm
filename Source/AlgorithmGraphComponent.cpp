/*
  ==============================================================================

    AlgorithmGraphComponent.cpp
    Created: 9 Apr 2021 11:32:49am
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "AlgorithmGraphComponent.h"

void OperatorBox::resized()
{
    filledBounds = getBounds().toFloat().reduced(fOffset);
}

void OperatorBox::paint(juce::Graphics &g)
{
    g.setColour(fillColor);
    g.fillRect(filledBounds);
    g.setColour(textColor);
    g.setFont(juce::Font("VCR OSD Mono", 15.0f, 0)); //TODO: pick a better font
    auto textBounds = filledBounds.reduced(5.0f);
    auto idxString = juce::String(std::to_string(index));
    g.drawText(idxString, textBounds, juce::Justification::centred);
}

void RoutingPath::addPathAt(int gridX, int gridY, RoutingDirection dir)
{
    auto startingPoint = AlgorithmGridConstants::getCellCenter(gridX, gridY);
    auto path = new juce::Path();
    path->startNewSubPath(startingPoint.first, startingPoint.second);
    switch(dir)
    {
        case RoutingDirection::down:
        {
            auto endPoint = AlgorithmGridConstants::getCellCenter(gridX, gridY + 1);
            path->lineTo(endPoint.first, endPoint.second);
            break;
        }
        case RoutingDirection::downLeft:
        {
            auto p1 = AlgorithmGridConstants::getCellCenter(gridX - 1, gridY);
            auto p2 = AlgorithmGridConstants::getCellCenter(gridX - 1, gridY + 1);
            path->lineTo(p1.first, p1.second);
            path->lineTo(p2.first, p1.second);
            break;
        }
        case RoutingDirection::downRight:
        {
            auto p1 = AlgorithmGridConstants::getCellCenter(gridX + 1, gridY);
            auto p2 = AlgorithmGridConstants::getCellCenter(gridX + 1, gridY + 1);
            path->lineTo(p1.first, p1.second);
            path->lineTo(p2.first, p1.second);
            break;
        }
        case RoutingDirection::left:
        {
            auto endPoint = AlgorithmGridConstants::getCellCenter(gridX - 1, gridY);
            path->lineTo(endPoint.first, endPoint.second);
            break;
        }
        case RoutingDirection::right:
        {
            auto endPoint = AlgorithmGridConstants::getCellCenter(gridX + 1, gridY);
            path->lineTo(endPoint.first, endPoint.second);
            break;
        }
        case RoutingDirection::selfMod:
        {
            auto yUp = startingPoint.second - (AlgorithmGridConstants::cellSideLength * 0.65f);
            auto xLeft = startingPoint.first - (AlgorithmGridConstants::cellSideLength * 0.65f);
            path->lineTo(xLeft, startingPoint.second);
            path->lineTo(xLeft, yUp);
            path->lineTo(startingPoint.first, yUp);
            break;
        }
    }
    path->closeSubPath();
    paths.push_back(*path);
}

void AlgorithmGraph::timerCallback()
{
    
}

void AlgorithmGraph::resized()
{
    
}

void AlgorithmGraph::paint(juce::Graphics &g)
{
    
}

