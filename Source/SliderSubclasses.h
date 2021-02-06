

#pragma once
#include <JuceHeader.h>
#include "RGBColor.h"

class EnvLabel : public juce::Label, public juce::Slider::Listener
{
public:
    EnvLabel(juce::Slider* sliderToAttach, juce::String suff) : attachSlider(sliderToAttach), suffix(suff)
    {
        auto vcrFont = juce::Font("WW Digital", 8.0f, 0);
        setEditable(true);
        getLookAndFeel().setDefaultSansSerifTypeface(getLookAndFeel().getTypefaceForFont(vcrFont));
        attachSlider->addListener(this);
        setJustificationType(juce::Justification::centred);
        setMinimumHorizontalScale(0.25);
        juce::String tempText;
        auto valueToDraw = attachSlider->getValue();
        auto fullString = juce::String(valueToDraw);
        if(valueToDraw < 100.0f)
            tempText = fullString.substring(0, 2);
        else if(valueToDraw < 1000.0f)
            tempText = fullString.substring(0, 3);
        else
            tempText = fullString.substring(0, 4);
        auto labelText = tempText +  suffix;
        setText(labelText, juce::dontSendNotification);
    }
    ~EnvLabel() {}
    void sliderValueChanged(juce::Slider* slider) override
    {
        juce::String tempText;
        juce::String labelText;
        auto valueToDraw = attachSlider->getValue();
        auto fullString = juce::String(valueToDraw);
        if(suffix == " ")
        {
            labelText = fullString;
        }
        else
        {
            if(valueToDraw < 100.0f)
                tempText = fullString.substring(0, 3);
            else if(valueToDraw < 1000.0f)
                tempText = fullString.substring(0, 4);
            else
                tempText = fullString.substring(0, 5);
            labelText = tempText +  suffix;
        }
        setText(labelText, juce::dontSendNotification);
    }
    void textWasEdited() override
    {
        auto str = getText(true);
        auto val = str.getDoubleValue();
        attachSlider->setValue(val);
    }
private:
    juce::Colour textColor = juce::Colours::white;
    juce::Slider* attachSlider;
    juce::String suffix;
};
class LevelSlider : public juce::Component
{
public:
    LevelSlider(int index) : opIndex(index)
    {
        slider.setSliderStyle(juce::Slider::LinearVertical);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 45, 15);
        addAndMakeVisible(slider);
    }
    ~LevelSlider(){}
    void attach(juce::AudioProcessorValueTreeState* pTree)
    {
        juce::String iStr = juce::String(opIndex);
        auto paramId = "levelParam" + iStr;
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*pTree, paramId, slider));
    }
    void resized() override
    {
        auto rect = getLocalBounds();
        slider.setBounds(rect);
    }
    juce::Slider slider;
    juce::String idStr;
    int opIndex;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class DelaySlider : public juce::Slider
{
public:
    DelaySlider(int index) : opIndex(index)
    {
        setTooltip("Delay Time");
        setSliderStyle(juce::Slider::Rotary);
        setTextBoxStyle(juce::Slider::NoTextBox, true,0, 0);
    }
    ~DelaySlider(){}
    void attach(juce::AudioProcessorValueTreeState* pTree)
    {
        juce::String iStr = juce::String(opIndex);
        auto paramId = "delayParam" + iStr;
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*pTree, paramId, *this));
    }
    juce::String idStr;
    int opIndex;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class AttackSlider : public juce::Slider
{
public:
    AttackSlider(int index) : opIndex(index)
    {
        setTooltip("Attack Time");
        setSliderStyle(juce::Slider::Rotary);
        setTextBoxStyle(juce::Slider::NoTextBox, true,0, 0);
        
    }
    ~AttackSlider(){}
    void attach(juce::AudioProcessorValueTreeState* pTree)
    {
        juce::String iStr = juce::String(opIndex);
        auto paramId = "attackParam" + iStr;
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*pTree, paramId, *this));
    }
    juce::String idStr;
    int opIndex;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class HoldSlider : public juce::Slider
{
public:
    HoldSlider(int index) : opIndex(index)
    {
        setTooltip("Hold Time");
        setSliderStyle(juce::Slider::Rotary);
        setTextBoxStyle(juce::Slider::NoTextBox, true,0, 0);
    }
    ~HoldSlider(){}
    void attach(juce::AudioProcessorValueTreeState* pTree)
    {
        juce::String iStr = juce::String(opIndex);
        auto paramId = "holdParam" + iStr;
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*pTree, paramId, *this));
    }
    juce::String idStr;
    int opIndex;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class DecaySlider : public juce::Slider
{
public:
    DecaySlider(int index) : opIndex(index)
    {
        setTooltip("Decay Time");
        setSliderStyle(juce::Slider::Rotary);
        setTextBoxStyle(juce::Slider::NoTextBox, true,0, 0);
    }
    ~DecaySlider(){}
    void attach(juce::AudioProcessorValueTreeState* pTree)
    {
        juce::String iStr = juce::String(opIndex);
        auto paramId = "decayParam" + iStr;
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*pTree, paramId, *this));
    }
    int opIndex;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class SustainSlider : public juce::Slider
{
public:
    SustainSlider(int index) : opIndex(index)
    {
        setTooltip("Sustain Level");
        setSliderStyle(juce::Slider::Rotary);
        setTextBoxStyle(juce::Slider::NoTextBox, true,0, 0);
    }
    ~SustainSlider(){}
    void attach(juce::AudioProcessorValueTreeState* pTree)
    {
        juce::String iStr = juce::String(opIndex);
        auto paramId = "sustainParam" + iStr;
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*pTree, paramId, *this));
    }
    juce::String idStr;
    int opIndex;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class ReleaseSlider : public juce::Slider
{
public:
    ReleaseSlider(int index) : opIndex(index)
    {
        setTooltip("Release Time");
        setSliderStyle(juce::Slider::Rotary);
        setTextBoxStyle(juce::Slider::NoTextBox, true,0, 0);
    }
    ~ReleaseSlider(){}
    void attach(juce::AudioProcessorValueTreeState* pTree)
    {
        juce::String iStr = juce::String(opIndex);
        auto paramId = "releaseParam" + iStr;
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*pTree, paramId, *this));
    }
    juce::String idStr;
    int opIndex;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class RatioSlider : public juce::Component
{
public:
    RatioSlider(int index) :  opIndex(index)
    {
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 45, 15);
        addAndMakeVisible(slider);
        
    }
    ~RatioSlider(){}
    void attach(juce::AudioProcessorValueTreeState* pTree)
    {
        juce::String iStr = juce::String(opIndex);
        auto paramId = "ratioParam" + iStr;
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*pTree, paramId, slider));
    }
    void resized() override
    {
        auto rect = getLocalBounds();
        slider.setBounds(rect);
    }
    juce::Slider slider;
    juce::String idStr;
    int opIndex;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class ModIndexSlider : public juce::Component
{
public:
    ModIndexSlider(int index) : opIndex(index)
    {
        slider.setTooltip("Maximum frequency modulation(Hz.)");
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 45, 15);
        addAndMakeVisible(slider);
    }
    ~ModIndexSlider(){}
    void attach(juce::AudioProcessorValueTreeState* pTree)
    {
        juce::String iStr = juce::String(opIndex);
        auto paramId = "indexParam" + iStr;
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(*pTree, paramId, slider));
    }
    void resized() override
    {
        slider.setBounds(getLocalBounds());
    }
    juce::Slider slider;
    juce::String idStr;
    int opIndex;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class OutputToggleButton : public juce::TextButton
{
public:
    OutputToggleButton(int index) : opIndex(index)
    {
        setClickingTogglesState(true);
        setTooltip("Toggles this operator's audio output");
        setButtonText("Output");
        outputOnColor = ColorCreator::RGBColor(226, 76, 86);
        outputOffColor = ColorCreator::RGBColor(31, 46, 50);
        setColour(juce::TextButton::buttonColourId, outputOffColor);
        setColour(juce::TextButton::buttonOnColourId, outputOnColor);
    }
    ~OutputToggleButton() {}
    void attach(juce::AudioProcessorValueTreeState* pTree)
    {
        juce::String iStr = juce::String(opIndex);
        auto paramId = "audibleParam" + iStr;
        attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(*pTree, paramId, *this));
    }
    
    int opIndex;
private:
    juce::Colour outputOffColor;
    juce::Colour outputOnColor;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;
};



