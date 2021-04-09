/*
  ==============================================================================

    OperatorProcessor.h
    Created: 6 Oct 2020 1:04:48pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "maximilian.h"
#include "DAHDSR.h"
#include "LfoProcessor.h"
#include "WavetableProcessor.h"

class Operator
{
public:
    Operator(int opIndex, int voiceIndex) : lastOutputSample(0.0f), envelope(opIndex), voice(voiceIndex), ratio(1.0f), index(opIndex)
    {
        minRatio = std::numeric_limits<float>::max();
        maxRatio = std::numeric_limits<float>::min();
    }
    ~Operator()
    {
        //printf("Minimum ratio: %f\n", minRatio);
        //printf("Maximum ratio: %f\n", maxRatio);
    }
    int getIndex()
    {
        return index;
    }
    void cleanFreqOffset()
    {
        modOffset = 0.0f;
    }
    void modulateRatio(float value, int mode);
    
    bool isActive()
    {
        if(envelope.getPhase() == DAHDSR::noteOff)
            return false;
        else
            return true;
    }
    void updateSampleRate(double newRate)
    {
        envelope.setSampleRate(newRate);
        wtOsc.setSampleRate(newRate);
    }
    float sample(float fundamental);
    float lastOutputSample;
    bool isAudible = false;
    DAHDSR envelope;
    int voice;
    float modOffset;
    float rawSample;
private:
    float ratio;
    float modIndex;
    float level;
    int index;
    SineTableOscillator wtOsc;
    const int ratioId = (3 * index) + 2;
    const int modIndexId = (3 * index) + 3;
    const int levelId = (3 * index) + 4;
    float minRatio;
    float maxRatio;
};
