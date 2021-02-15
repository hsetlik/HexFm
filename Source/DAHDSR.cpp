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
                if(FmSynthParams::opDelayTime[index] > 0)
                {
                    if(samplesIntoPhase == 0)
                        samplesInPhase = floor(FmSynthParams::opDelayTime[index] * (sampleRate / 1000));
                    samplesIntoPhase += 1;
                    if(samplesIntoPhase >= samplesInPhase)
                    {
                        currentPhase = attackPhase;
                        samplesIntoPhase = 0;
                        samplesInPhase = floor(FmSynthParams::opAttackTime[index] * (sampleRate / 1000));
                        factor = exp((log(1.0f) - log(minLevel)) /samplesInPhase);
                    }
                    output = 0.0f;
                }
                else
                {
                    currentPhase = attackPhase;
                    samplesInPhase = floor(FmSynthParams::opAttackTime[index] * (sampleRate / 1000));
                    factor = exp((log(1.0f) - log(minLevel)) /samplesInPhase);
                    samplesIntoPhase = 0;
                }
                break;
            }
            case attackPhase:
            {
                if(samplesIntoPhase == 0)
                    output = minLevel;
                output = output * factor;
                samplesIntoPhase++;
                if(samplesIntoPhase > samplesInPhase)
                {
                    currentPhase = holdPhase;
                    samplesIntoPhase = 0;
                    samplesInPhase = FmSynthParams::opHoldTime[index] * (sampleRate / 1000);
                }
                break;
            }
            case holdPhase:
            {
                if(FmSynthParams::opHoldTime[index] != 0)
                {
                    samplesIntoPhase += 1;
                    if(samplesIntoPhase > samplesInPhase)
                    {
                        currentPhase = decayPhase;
                        samplesIntoPhase = 0;
                        samplesInPhase = FmSynthParams::opDecayTime[index] * (sampleRate / 1000);
                        factor = exp((log(FmSynthParams::opSustainLevel[index]) - log(1.0f)) /samplesInPhase);
                    }
                    output = 1.0f;
                }
                else
                {
                    currentPhase = decayPhase;
                    samplesIntoPhase = 0;
                    samplesInPhase = FmSynthParams::opDecayTime[index] * (sampleRate / 1000);
                    factor = exp((log(FmSynthParams::opSustainLevel[index]) - log(1.0f)) /samplesInPhase);;
                }
                break;
            }
            case decayPhase:
            {
                output = output * factor;
                samplesIntoPhase += 1;
                if(samplesIntoPhase >= samplesInPhase)
                {
                    currentPhase = sustainPhase;
                    samplesIntoPhase = 0;
                    output = FmSynthParams::opSustainLevel[index];
                }
                break;
            }
            case sustainPhase:
            {
                output = FmSynthParams::opSustainLevel[index];
                break;
            }
            case releasePhase:
            {
                output = output * factor;
                samplesIntoPhase += 1;
                if(samplesIntoPhase > samplesInPhase)
                    currentPhase = noteOff;
                break;
            }
            case noteOff:
            {
                samplesIntoPhase = 0;
                output = 0.0f;
                break;
            }
            default:
                break;
        }
    /*
    if(output > 0.0)
        printf("envelope on\n");
     */
    return input * output;
}
