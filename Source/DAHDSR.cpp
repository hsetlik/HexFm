/*
  ==============================================================================

    DAHDSR.cpp
    Created: 6 Oct 2020 1:07:11pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "DAHDSR.h"

float DAHDSR::process(float input)
{
        switch(currentPhase)
        {
            case delayPhase:
            {
                if(decay != 0)
                {
                    int samplesInPhase = floor(delay * (sampleRate / 1000));
                    samplesIntoPhase += 1;
                    if(samplesIntoPhase > samplesInPhase)
                    {
                        currentPhase = attackPhase;
                        samplesIntoPhase = 0;
                    }
                    return 0.0f * input;
                }
                else
                    currentPhase = attackPhase;
            }
            case attackPhase:
            {
                int samplesInPhase = floor(attack * (sampleRate / 1000));
                auto sampleDelta = 1.0f / samplesInPhase;
                auto factor = samplesIntoPhase * sampleDelta;
                samplesIntoPhase += 1;
                if(samplesIntoPhase > samplesInPhase)
                {
                    currentPhase = holdPhase;
                    samplesIntoPhase = 0;
                }
                return input * factor;
                
            }
            case holdPhase:
            {
                if(hold != 0)
                {
                    int samplesInPhase = floor(hold * (sampleRate / 1000));
                    samplesIntoPhase += 1;
                    if(samplesIntoPhase > samplesInPhase)
                    {
                        currentPhase = decayPhase;
                        samplesIntoPhase = 0;
                    }
                    return input;
                }
                else
                    currentPhase = decayPhase;
            }
            case decayPhase:
            {
                int samplesInPhase = floor(decay * (sampleRate / 1000));
                auto sampleDelta = (1.0f - sustainLevel) / samplesInPhase;
                auto factor = 1.0f - (samplesIntoPhase * sampleDelta);
                samplesIntoPhase += 1;
                if(samplesIntoPhase > samplesInPhase)
                {
                    currentPhase = sustainPhase;
                    samplesIntoPhase = 0;
                }
                return (input * factor);
                
            }
            case sustainPhase:
            {
                return input * sustainLevel;
            }
            case releasePhase:
            {
                int samplesInPhase = floor(release * (sampleRate / 1000));
                auto sampleDelta = sustainLevel / samplesInPhase;
                auto factor = sustainLevel - (sampleDelta * samplesIntoPhase);
                samplesIntoPhase += 1;
                if(samplesIntoPhase > samplesInPhase)
                    currentPhase = noteOff;
                return input * factor;
            }
            case noteOff:
            {
                samplesIntoPhase = 0;
                return 0.0f;
            }
        }
}
