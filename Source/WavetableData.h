/*
  ==============================================================================

    WavetableData.h
    Created: 1 Mar 2021 1:31:46pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#define TABLESIZE 512
#define TABLES_PER_FRAME 10
enum WaveType
{
    Sine,
    Square,
    Saw,
    Tri
};

class WTArray
{
public:
    static std::array<float, TABLESIZE> makeArray(WaveType type)
    {
        std::array<float, TABLESIZE> arr;
        switch(type)
        {
            case Sine:
            {
                auto dPhase = juce::MathConstants<float>::twoPi / (float)TABLESIZE;
                for(int i = 0; i < TABLESIZE; ++i)
                {
                    arr[i] = std::sin(dPhase * (float)i);
                }
                break;
            }
            case Square:
            {
                for(int i = 0; i < TABLESIZE; ++i)
                {
                    if(i < (TABLESIZE / 2))
                        arr[i] = -1.0f;
                    else
                        arr[i] = 1.0f;
                }
                break;
            }
            case Saw:
            {
                auto dLevel = 2.0f / (float)TABLESIZE;
                for(int i = 0; i < TABLESIZE; ++i)
                {
                    arr[i] = -1.0f + (dLevel * i);
                }
                break;
            }
            case Tri:
            {
                auto dLevel = 4.0f / (float)TABLESIZE;
                float level = -1.0f;
                for(int i = 0; i < TABLESIZE; ++i)
                {
                    if(i < TABLESIZE / 2)
                        level += dLevel;
                    else
                        level -= dLevel;
                    arr[i] = level;
                }
                break;
            }
        }
        return arr;
    }
    
};
