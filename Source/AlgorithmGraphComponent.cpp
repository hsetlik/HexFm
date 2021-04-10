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
    toDraw.reserve(sizeof(OperatorInfo*) * 6);
    for(int s = 0; s < 6; ++s)
    {
        for(int d = 0; d < 6; ++d)
        {
            if(ParamStatic::opRouting[s][d] != 0)
            {
                VectorUtil::addIfUnique(opInfo[d].sources, &opInfo[s]);
                VectorUtil::addIfUnique(opInfo[s].dests, &opInfo[d]);
                VectorUtil::addIfUnique(toDraw, &opInfo[d]);
                VectorUtil::addIfUnique(toDraw, &opInfo[s]);
            }
        }
    }
    bottomLevel.clear();
    bottomLevel.reserve(sizeof(OperatorInfo*) * 6);
    for(auto op : opInfo)
    {
        if(op.sources.size() > 0 && op.dests.size() < 1)
        {
            VectorUtil::addIfUnique(bottomLevel, &op);
            op.modOrder = 0;
        }
    }
}
void AlgorithmGraph::calculateRows()
{
    for(auto op : bottomLevel)
    {
        op->setModOrder();
    }
    int totalRows = 0;
    for(auto op : toDraw)
    {
        if(op->modOrder > totalRows && op->modOrder < 6)
            totalRows = op->modOrder;
    }
    printf("%d total rows\n", totalRows);
}
void AlgorithmGraph::paint(juce::Graphics &g)
{
    initOperators();
    updateOpInfo();
    auto b = &bottomLevel;
    //gridRows.clear();
    calculateRows();
}

