/*
  ==============================================================================

    WaveTypeSelector.h
    Created: 4 Jun 2021 8:44:02am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include "RGBColor.h"
#include "GlobalColor.h"
#include "WavetableProcessor.h"

class ImageToggle : public juce::ImageButton
{
public:
    ImageToggle(juce::Image on, juce::Image off);
    void paintButton(juce::Graphics &g, bool down, bool highlighted) override;
    static juce::Image getForWave(WaveType type, bool on)
    {
        switch(type)
        {
            case Sine:
            {
                if(on)
                    return juce::ImageCache::getFromMemory(BinaryData::sineOn_png, BinaryData::sineOn_pngSize);
                else
                    return juce::ImageCache::getFromMemory(BinaryData::sineOff_png, BinaryData::sineOff_pngSize);
            }
            case Square:
            {
                if(on)
                    return juce::ImageCache::getFromMemory(BinaryData::squareOn_png, BinaryData::squareOn_pngSize);
                else
                    return juce::ImageCache::getFromMemory(BinaryData::squareOff_png, BinaryData::squareOff_pngSize);
            }
            case Saw:
            {
                if(on)
                    return juce::ImageCache::getFromMemory(BinaryData::sawOn_png, BinaryData::sawOn_pngSize);
                else
                    return juce::ImageCache::getFromMemory(BinaryData::sawOff_png, BinaryData::sawOff_pngSize);
            }
            case Tri:
            {
                if(on)
                    return juce::ImageCache::getFromMemory(BinaryData::triOn_png, BinaryData::triOn_pngSize);
                else
                    return juce::ImageCache::getFromMemory(BinaryData::triOff_png, BinaryData::triOff_pngSize);
            }
        }
    }
private:
    juce::Image onImg;
    juce::Image offImg;
};


class WaveButtonSet :
public juce::Component,
public juce::Button::Listener,
public juce::ComboBox::Listener
{
public:
    WaveButtonSet(int opIndex, juce::AudioProcessorValueTreeState* tree);
    void resized() override;
    void buttonClicked(juce::Button* b) override;
    void comboBoxChanged(juce::ComboBox* box) override;
private:
    WaveType currentType;
    ImageToggle sineButton;
    ImageToggle squareButton;
    ImageToggle sawButton;
    ImageToggle triButton;
    juce::ComboBox hiddenBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveAttach;
};
