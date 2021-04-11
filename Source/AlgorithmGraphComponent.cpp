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
    g.setFont(juce::Font("VCR OSD Mono", AlgorithmGridConstants::opTextSize, 0)); //TODO: pick a better font
    auto textBounds = filledBounds.reduced(5.0f);
    auto idxString = juce::String(std::to_string(index + 1));
    g.drawText(idxString, textBounds, juce::Justification::centred);
}

void AlgorithmGraph::addPath(std::pair<int, int> from, std::pair<int, int> to)
{
    paths.add(new juce::Path());
    auto path = paths.getLast();
    auto p1 = getCellCenter(from.first, from.second);
    auto p3 = getCellCenter(to.first, to.second);
    path->startNewSubPath(p1.first, p1.second);
    path->lineTo(p3.first, p3.second);
}

void AlgorithmGraph::timerCallback()
{
     
    if(ParamStatic::routingHasChanged)
    {
        repaint();
        ParamStatic::routingHasChanged = false;
    }
}

void AlgorithmGraph::resized()
{
    auto dL = AlgorithmGridConstants::cellSideLength;
    for(auto op : opBoxes)
    {
        op->setBounds(op->xPos * dL, op->yPos * dL, dL, dL);
        if(!op->isShowing())
            printf("Op not showing\n");
    }
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
                VectorUtil::addIfUnique(opInfo[d]->sources, opInfo[s]);
                VectorUtil::addIfUnique(opInfo[s]->dests, opInfo[d]);
                opInfo[s]->isActive = true;
                opInfo[d]->isActive = true;
                VectorUtil::addIfUnique(toDraw, opInfo[d]);
                VectorUtil::addIfUnique(toDraw, opInfo[s]);
            }
        }
    }
    for(auto op : toDraw)
    {
        //bottom row is any operator we can hear
        if(ParamStatic::opAudible[op->index])
        {
            bottomLevel.push_back(op);
        }
        //top row are active operators not modulated by anything
        if(op->sources.size() < 1 && (!VectorUtil::contains(bottomLevel, op)))
        {
            topLevel.push_back(op);
        }
    }
}
int AlgorithmGraph::calculateRows()
{
    int numRows = 0;
    gridRows.clear();
    if(toDraw.size() > 0)
    {
        bool silentFound = false;
        bool checkForSilent = false;
        if(bottomLevel.size() > 0)
            ++numRows;
        else
            checkForSilent = true;
        for(auto op : toDraw)
        {
            if(op->dests.size() < 1)
            {
                VectorUtil::addIfUnique(bottomLevel, op);
                silentFound = true;
            }
        }
        if(silentFound && checkForSilent)
            ++numRows;
        //figure out which operators belong in which rows starting from the top level
        if(topLevel.size() > 0)
            ++numRows;
        bool foundBottom = false;
        auto* currentLevel = &topLevel;
        
        while(!foundBottom)
        {
            //copy the current level vector and add it to gridRows
            foundBottom = true;
            auto newVec = std::vector<OpInfo*>(*currentLevel);
            gridRows.push_back(newVec);
            for(auto op : *currentLevel)
            {
                for(auto dest : op->dests)
                {
                    if(!VectorUtil::contains(bottomLevel, dest))
                        foundBottom = false; //the bottom is not found if any destinations are not not on the bottom level
                }
            }
            //if the bottom still isn't found, add another layer to the grid
            if(!foundBottom)
            {
                auto newRow = new std::vector<OpInfo*>();
                for(auto op : *currentLevel)
                {
                    for(auto dest : op->dests)
                    {
                        if(!VectorUtil::contains(bottomLevel, dest)) //we have to account for the bottom level: only lower row that's already determined
                            VectorUtil::addIfUnique(*newRow, dest);
                    }
                }
                currentLevel = newRow;
                ++numRows;
            }
        }
        gridRows.push_back(bottomLevel);
    }
    return numRows;
}
void AlgorithmGraph::paint(juce::Graphics &g)
{
    auto fBounds = getLocalBounds().toFloat();
    AlgorithmGridConstants::topLeftX = fBounds.getX();
    AlgorithmGridConstants::topLeftY = fBounds.getY();
    g.fillAll(background);
    reInitOpInfo();
    updateOpInfo();
    int rowCount = calculateRows();
    int largestDimension = rowCount;
    for(auto i : gridRows)
    {
        juce::String str = "In This Row: ";
        for(auto op : i)
        {
            auto numStr = juce::String(op->index);
            numStr += ", ";
            str += numStr;
        }
        str += "\n";
        //printf("%s", str.toRawUTF8());
        if(i.size() > largestDimension)
            largestDimension = (int)i.size();
    }
    //printf("\n");
    //increment the largest dimension so we have a bit of space around the edges
    largestDimension += 2;
    AlgorithmGridConstants::unitWidth = largestDimension;
    AlgorithmGridConstants::opTextSize = (float)60.0f / largestDimension;
    AlgorithmGridConstants::cellSideLength = fBounds.getHeight() / (float)largestDimension;
    opBoxes.clear();
    int currentRow = 1;
    for(auto row : gridRows)
    {
        int currentColumn = 1;
        for(auto op : row)
        {
            op->gridX = currentColumn;
            op->gridY = currentRow;
            addOperatorComponent(currentColumn, currentRow, op->index);
            ++currentColumn;
        }
        ++currentRow;
    }
    paths.clear();
    g.setColour(pathColor);
    auto strokeWeight = juce::PathStrokeType(AlgorithmGridConstants::routingStrokeWidth);
    for(auto source : toDraw)
    {
        auto startPoint = std::make_pair(source->gridX, source->gridY);
        for(auto dest : source->dests)
        {
            auto endPoint = std::make_pair(dest->gridX, dest->gridY);
            addPath(startPoint, endPoint);
            g.strokePath(*paths.getLast(), strokeWeight);
        }
    }
    for(auto op : opBoxes)
    {
        op->paint(g);
    }
    resized();
}

