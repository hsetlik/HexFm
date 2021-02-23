/*
  ==============================================================================

    WaveGraph.h
    Created: 15 Feb 2021 4:24:31pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ParameterStructure.h"
#include "RGBColor.h"
const int WAVE_RES = 128;
const int PERIOD = 64;

struct WaveData
{
    WaveData(float r, float l, int res) : angles(TOTAL_OPERATORS, 0.0f), baseAngleDelta(juce::MathConstants<float>::twoPi / WAVE_RES)
    {
        isVisible = true;
        for(int i = 0; i < TOTAL_OPERATORS; ++i)
        {
            std::vector<float> opPoints(WAVE_RES, 0.0);
            points.push_back(opPoints);
        }
    }
    float ratio;
    float level;
    bool isVisible;
    double actualFreq;
    double angleDelta;
    double output;
    std::vector<std::vector<float>> points;
    std::vector<float> angles;
    float baseAngleDelta;
};

class WaveGraph : public juce::Component, juce::Timer
{
public:
    WaveGraph() : opData(0.0, 0.0, WAVE_RES)
    {
        startTimerHz(24);
        for(int i = 0; i < TOTAL_OPERATORS; ++i)
        {
            opPaths.add(new juce::Path());
        }
    }
    ~WaveGraph() {}
    void timerCallback() override
    {
        updatePaths();
        //repaint();
    }
    void updatePaths();
    void paint(juce::Graphics& g) override;
    void resized() override
    {setBounds(getLocalBounds());}
private:
    double fullAmplitude = 85.0f;
    WaveData opData;
    juce::OwnedArray<juce::Path> opPaths;
    juce::Path audioPath;
    
    
};
