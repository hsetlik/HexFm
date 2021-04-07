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
    Operator(int opIndex, int voiceIndex) : envelope(opIndex), voice(voiceIndex), ratio(1.0f), index(opIndex), wtOsc(sine512)
    {
        
    }
    ~Operator() {}
    int getIndex()
    {
        return index;
    }
    void cleanFreqOffset()
    {
        modOffset = 0.0f;
    }
    void modulateRatio(float value, int mode)
    {
        //value is between 0 and 1
        //min ratio is 0.1f, max is 10.0f
        auto maxIncrease = 10.0f - ratio;
        auto maxDecrease = ratio - 0.1f;
        float modValue;
        switch(mode)
        {
            case 0: //upwards only
            {
                modValue = maxIncrease * value;
                break;
            }
            case 1: // both directions
            {
                auto biValue = (value * 2.0f) - 1.0f; //track value to range -1, 1
                if(biValue > 0.0f)
                    modValue = maxIncrease * biValue;
                else
                    modValue = maxDecrease * biValue;
                break;
            }
            case 2: //downwards only
            {
                modValue =  -maxDecrease * value;
                break;
            }
        }
        ParamStatic::opRatioMod[index] = modValue;
    }
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
    WToscillator wtOsc;
    const int ratioId = (3 * index) + 2;
    const int modIndexId = (3 * index) + 3;
    const int levelId = (3 * index) + 4;
};
