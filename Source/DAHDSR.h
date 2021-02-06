/*
  ==============================================================================

    DAHDSR.h
    Created: 6 Oct 2020 1:07:11pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <math.h>

class DAHDSR
{
    enum envPhase
    {
        delayPhase,
        attackPhase,
        holdPhase,
        decayPhase,
        sustainPhase,
        releasePhase,
        noteOff
    };
public:
    //functions
    DAHDSR() : sampleRate(44100)
    {
        trigger = false;
        samplesIntoPhase = 0;
        currentPhase = noteOff;
    }
    ~DAHDSR() {}
    void triggerOn()
    {
        trigger = true;
        currentPhase = delayPhase;
    }
    void triggerOff()
    {
        trigger = false;
        currentPhase = releasePhase;
        samplesIntoPhase = 0;
    }
    //setters-- all time values are in ms
    void setSampleRate(double value) {sampleRate = value;}
    void setDelay(float value) {delay = value;}
    void setAttack(float value) {attack = value;}
    void setHold(float value) {hold = value;}
    void setDecay(float value) {decay = value;}
    void setSustain(float value) {sustainLevel = value;}
    void setRelease(float value) {release = value;}
    //getters
    float process(float input);
    float getDelay() {return delay;}
    float getAttack() {return attack;}
    float getHold() {return hold;}
    float getDecay() {return decay;}
    float getSustain() {return sustainLevel;}
    float getRelease() {return release;}
private:
    //data
    envPhase currentPhase;
    int samplesIntoPhase;
    float delay;
    float attack;
    float hold;
    float decay;
    float sustainLevel;
    float release;
    double sampleRate;
    bool trigger;
};
