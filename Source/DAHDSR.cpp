/*
  ==============================================================================

    DAHDSR.cpp
    Created: 6 Oct 2020 1:07:11pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "DAHDSR.h"

float DAHDSR::factorFor(float startLevel, float endLevel, float lengthMs)
{
    if(startLevel == 0.0f)
        startLevel = minLevel;
    if(endLevel == 0.0f)
        endLevel = minLevel;
    unsigned long phaseLengthSamples = lengthMs * (sampleRate / 1000);
    return exp((log(endLevel) - log(startLevel)) / phaseLengthSamples);
}

void DAHDSR::enterPhase(envPhase newPhase)
{
    currentPhase = newPhase;
    samplesIntoPhase = 0;
    switch(newPhase)
    {
        case delayPhase:
        {
            startLevel = minLevel;
            endLevel = minLevel;
            samplesInPhase = delayTime * (sampleRate / 1000);
            factor = factorFor(startLevel, endLevel, delayTime);
            break;
        }
        case attackPhase:
        {
            startLevel = minLevel;
            endLevel = 1.0f;
            samplesInPhase = attackTime * (sampleRate / 1000);
            factor = factorFor(startLevel, endLevel, attackTime);
            break;
        }
        case holdPhase:
        {
            startLevel = 1.0f;
            endLevel = 1.0f;
            samplesInPhase = holdTime * (sampleRate / 1000);
            factor = factorFor(startLevel, endLevel, holdTime);
            break;
        }
        case decayPhase:
        {
            startLevel = 1.0f;
            endLevel = sustainLevel;
            samplesInPhase = decayTime * sampleRate / 1000;
            factor = factorFor(startLevel, endLevel, decayTime);
            break;
        }
        case sustainPhase:
        {
            startLevel = sustainLevel;
            endLevel = sustainLevel;
            samplesInPhase = 1000000;
            factor = 1.0f;
            break;
        }
        case releasePhase:
        {
            startLevel = sustainLevel;
            endLevel = minLevel;
            samplesInPhase = releaseTime * sampleRate / 1000;
            factor = factorFor(startLevel, endLevel, releaseTime);
            break;
        }
        case noteOff:
        {
            startLevel = minLevel;
            endLevel = minLevel;
            samplesInPhase = 100000000;
            factor = 0.0f;
            break;
        }
    }
    output = startLevel;
    updatePhase();
}

float DAHDSR::process(float input)
{
    updatePhase();
    ++samplesIntoPhase;
    output *= factor;
    return input * output;
}
