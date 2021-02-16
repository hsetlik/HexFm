/*
  ==============================================================================

    WaveGraph.cpp
    Created: 15 Feb 2021 4:24:31pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "WaveGraph.h"

void WaveGraph::updatePaths()
{
    opData.baseAngleDelta = juce::MathConstants<float>::twoPi / WAVE_RES;
    for(int i = 1; i < WAVE_RES + 1; ++i)
    {
        for(int op = 0; op < TOTAL_OPERATORS; ++op)
        {
            opData.actualFreq = FmSynthParams::workingFundamental * FmSynthParams::opRatio[op];
            for(int src = 0; src < TOTAL_OPERATORS; ++src)
            {
                if(FmSynthParams::opRouting[src][op] == 1)
                    opData.actualFreq += opData.points[src][i - 1] * FmSynthParams::opModIndex[op];
            }
            opData.level = FmSynthParams::opEnvLevel[op] * FmSynthParams::opLevel[op] * FmSynthParams::opAmplitudeMod[op];
            opData.angleDelta = (opData.baseAngleDelta * opData.actualFreq) / FmSynthParams::workingFundamental;
            opData.angles[op] += opData.angleDelta;
            opData.output = std::sin(opData.angles[op]);
            opData.points[op][i] = opData.output * opData.level;
        }
    }
}


void WaveGraph::paint(juce::Graphics &g)
{
    if(isVisible())
    {
        auto graphArea = getLocalBounds().reduced(6).toFloat();
        auto xDelta = graphArea.getWidth() / WAVE_RES;
        auto yCenter = 6.0 + graphArea.getHeight() / 2;
        audioPath.startNewSubPath(0, getHeight());
        for(auto* p : opPaths)
        {
            p->clear();
            p->startNewSubPath(0, getHeight());
        }
            
        for(int i = 0; i < WAVE_RES; ++i)
        {
            float audioPoint = yCenter;
            auto currentX = 6 + (xDelta * i);
            int op = 0;
            for(auto* p : opPaths)
            {
                p->lineTo(currentX, yCenter + (fullAmplitude * opData.points[op][i]));
                if(FmSynthParams::opAudible[op])
                    audioPoint += (fullAmplitude * opData.points[op][i]);
                ++op;
            }
            audioPath.lineTo(currentX, audioPoint);
        }
        g.setColour(juce::Colours::black);
        auto strokeType = juce::PathStrokeType(1.0f);
        audioPath.lineTo(getWidth(), getHeight());
        audioPath.closeSubPath();
        for(auto* p : opPaths)
        {
            p->lineTo(getWidth(), getHeight());
            p->closeSubPath();
            g.strokePath(*p, strokeType);
        }
        g.strokePath(audioPath, strokeType);
    }
}
