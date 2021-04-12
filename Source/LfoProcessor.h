/*
  ==============================================================================

    LfoProcessor.h
    Created: 9 Oct 2020 9:51:25am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "maximilian.h"
#include "SampleAndHoldOscillator.h"
#include "ParameterStructure.h"
#include "WavetableProcessor.h"
//const to set the synth's total availible number of LFOs
/* GUIDE TO TARGET NUMBERS
    Operator : Parameter : id
    NO TARGET    :         1
    0        : ratio     : 2
    0        : modIndex  : 3
    0        : level     : 4
    1        : ratio     : 5
    1        : modIndex  : 6
    1        : level     : 7
    2        : ratio     : 8
    2        : modIndex  : 9
    2        : level     : 10
    3        : ratio     : 11
    3        : modIndex  : 12
    3        : level     : 13
    4        : ratio     : 14
    4        : modIndex  : 15
    4        : level     : 16
    5        : ratio     : 17
    5        : modIndex  : 18
    5        : level     : 19
    ratioInt = 3 * opIndex + 2
    modIndex int = 3 * opIndex + 3
    levelInt = 3 * opIndex + 4
 */

const int totalLfos = 4;

class LfoProcessor
{
public:
    juce::AudioProcessorValueTreeState* tree;
    //functions
    LfoProcessor(int index, juce::AudioProcessorValueTreeState* t);
    ~LfoProcessor() {}
    void updateParams();
    float getValue(juce::String str)
    {
        return *tree->getRawParameterValue(str);
    }
    float getSampleValue();
    //data
    int currentTarget;
    int currentWaveType;
    float currentRate;
    float currentLevel;
    int getIndex() {return lfoIndex;}
    float lastValue;
    float rate;
    float level;
    int target;
    int wave;
    int ratioModType;
private:
    juce::String rateId;
    juce::String levelId;
    juce::String targetId;
    juce::String waveId;
    juce::String ratioModId;
    maxiOsc lfo_osc;
    RandomOscillator randOsc;
    int lfoIndex;
    
    
};
