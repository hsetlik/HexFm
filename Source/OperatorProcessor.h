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

class Operator
{
public:
    Operator(int opIndex) : envelope(opIndex), ratio(1.0f), index(opIndex)
    {
        
    }
    ~Operator() {}
    int getIndex()
    {
        return index;
    }
    void cleanOffset()
    {
        modOffset = 0.0f;
    }
    void setAmpMod(float newVal)
    {
        amplitudeMod = newVal;
    }
    bool isActive()
    {
        if(envelope.getPhase() == DAHDSR::noteOff)
            return false;
        else
            return true;
    }
    float sample(float fundamental);
    float lastOutputSample;
    bool isAudible = false;
    DAHDSR envelope;
    float modOffset;
    float rawSample;
private:
    float amplitudeMod = 1.0f;
    float ratio;
    float modIndex;
    float level;
    int index;
    maxiOsc osc;
    const int ratioId = (3 * index) + 2;
    const int modIndexId = (3 * index) + 3;
    const int levelId = (3 * index) + 4;
};
