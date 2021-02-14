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
public:
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
        samplesInPhase = floor(delay * (sampleRate / 1000));
        samplesIntoPhase = 0;
        currentPhase = delayPhase;
    }
    void triggerOff()
    {
        trigger = false;
        currentPhase = releasePhase;
        samplesIntoPhase = 0;
        samplesInPhase = release * (sampleRate / 1000);
        factor = 1.0 + (log(minLevel) - log(sustainLevel)) / (samplesInPhase);
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
    envPhase getPhase() {return currentPhase;}
private:
    //data
    envPhase currentPhase;
    unsigned long long samplesIntoPhase;
    unsigned long long samplesInPhase;
    double output;
    double factor;
    float minLevel = 0.00001f;
    float delay;
    float attack;
    float hold;
    float decay;
    float sustainLevel;
    float release;
    double sampleRate;
    bool trigger;
};
