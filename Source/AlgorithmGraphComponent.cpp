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

void AlgorithmGraph::addPath(std::pair<int, int> from, std::pair<int, int> to)
{
    auto path = new juce::Path();
    auto p1 = getCellCenter(from.first, from.second);
    auto p2 = getCellCenter(to.first, from.second);
    auto p3 = getCellCenter(to.first, to.second);
    path->startNewSubPath(p1.first, p1.second);
    path->lineTo(p2.first, p2.second);
    path->lineTo(p3.first, p3.second);
    path->closeSubPath();
    paths.push_back(*path);
}

void AlgorithmGraph::timerCallback()
{
    auto check = ParamStatic::routingHasChanged;
    if(check == true)
    {
        repaint();
        ParamStatic::routingHasChanged = false;
    }
}

void AlgorithmGraph::resized()
{
    
}

void AlgorithmGraph::updateOpInfo()
{
    toDraw.clear();
    bottomLevel.clear();
    topLevel.clear();
    for(int s = 0; s < 6; ++s)
    {
        for(int d = 0; d < 6; ++d)
        {
            if(ParamStatic::opRouting[s][d])
            {
                opInfo[d]->sources.push_back(opInfo[s]);
                opInfo[s]->dests.push_back(opInfo[d]);
                opInfo[s]->isActive = true;
                opInfo[d]->isActive = true;
                VectorUtil::addIfUnique(toDraw, opInfo[d]);
                VectorUtil::addIfUnique(toDraw, opInfo[s]);
            }
        }
    }
    for(auto op : toDraw)
    {
        if(ParamStatic::opAudible[op->index])
        {
            bottomLevel.push_back(op);
        }
        if(op->sources.size() < 1 && (!VectorUtil::contains(bottomLevel, op)))
        {
            topLevel.push_back(op);
        }
    }
    auto tl = &topLevel;
    auto bl = &bottomLevel;
}
void AlgorithmGraph::calculateRows()
{
    
}
void AlgorithmGraph::paint(juce::Graphics &g)
{
    updateOpInfo();
}

