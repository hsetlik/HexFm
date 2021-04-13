/*
  ==============================================================================

    CustomLnF.cpp
    Created: 7 Apr 2021 12:21:29am
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "CustomLnF.h"

void LnF1::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &s)
{
    auto iBounds = juce::Rectangle<int> {x, y, width, height};
    auto fBounds = iBounds.toFloat().reduced(5.8f);
    g.setColour(UXPalette::darkGray3);
    g.fillEllipse(fBounds);
    g.setColour(UXPalette::lightGray);
    auto angle = fabs(rotaryStartAngle - rotaryEndAngle) * sliderPos;
    auto centerX = fBounds.getX() + (fBounds.getWidth() / 2.0f);
    auto centerY = fBounds.getY() + (fBounds.getHeight() / 2.0f);
    auto radius = fBounds.getWidth() * 0.4f;
    auto strokeType = juce::PathStrokeType(5.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
    juce::Path track;
    track.addCentredArc(centerX, centerY, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.strokePath(track, strokeType);
    g.setColour(UXPalette::highlight);
    auto iRadius = radius * 0.6f;
    juce::Path thumb;
    thumb.startNewSubPath(centerX, centerY - radius);
    thumb.lineTo(centerX, centerY - iRadius);
    thumb.closeSubPath();
    thumb.applyTransform(juce::AffineTransform::rotation(rotaryStartAngle + angle, centerX, centerY));
    g.strokePath(thumb, strokeType);
}

void LnF1::drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos
                            , float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider &slider)
{
    auto fPos = slider.getValue() / (slider.getMaximum() - slider.getMinimum() + 0.00001f);
    auto fBounds = slider.getBounds().toFloat();
    auto bkgndWidth = fBounds.getWidth() * 0.2f;
    auto xOffsetBkgnd = (fBounds.getWidth() / 2.0f) - (bkgndWidth / 2.0f);
    auto corner = bkgndWidth / 3.5f;
    auto thumbWidth = fBounds.getWidth() * 0.45f;
    
    auto thumbXOffset = (fBounds.getWidth() / 2.0f) - (thumbWidth / 2.0f);
    auto thumbHeight = (fBounds.getHeight() - slider.getTextBoxHeight()) * 0.1f;
    auto thumbY = (1.0f - fPos) * ((fBounds.getHeight() - slider.getTextBoxHeight()) - thumbHeight - 5);
    thumbY += 5;
    
    /*
    printf("Slider Position: %f\n", sliderPos);
    printf("Thumb is at: %f\n", thumbY);
     */
    //draw the background
    g.setColour(UXPalette::darkGray1);
    g.fillRoundedRectangle(x + xOffsetBkgnd,
                           5,
                           bkgndWidth,
                           fBounds.getHeight() - (slider.getTextBoxHeight() * 1.2f) - 5,
                           corner);
    //draw the thumb
    g.setColour(UXPalette::highlight);
    g.fillRoundedRectangle(x + thumbXOffset,
                           thumbY,
                           thumbWidth,
                           thumbHeight,
                           corner);
    
    
    
}
