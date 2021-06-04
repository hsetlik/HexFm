/*
  ==============================================================================

    GlobalColor.h
    Created: 9 Apr 2021 6:15:55pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include "RGBColor.h"
namespace UXPalette {
static juce::Colour darkGray1 = Color::RGBColor(37, 50, 53);
static juce::Colour darkGray2 = Color::RGBColor(50, 80, 90);
static juce::Colour lightGray = Color::RGBColor(125, 126, 129);
static juce::Colour darkGray3 = lightGray.darker(0.7f);
static juce::Colour darkGray4 = lightGray.darker(0.9f);
static juce::Colour darkGray5 = darkGray4.darker(0.3f);
static juce::Colour lightRed = Color::RGBColor(226, 76, 85);
static juce::Colour darkRed = lightRed.darker(0.45f);
static juce::Colour lightOrange = Color::RGBColor(249, 171, 62);
static juce::Colour highlight = Color::RGBColor(255, 236, 95);
static juce::Colour nearBlack = Color::RGBColor(24, 24, 25);
/*
static juce::String operatorLabelFontName = "SquareFont Outline";
static juce::String modGridFontName = "Roboto Light";
 */
//==============================================================================
//! to easily get fonts from BinaryData
static juce::Font squareOutline(float height = 8.0f)
{
    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Squareo_ttf, BinaryData::Squareo_ttfSize);
    auto font = juce::Font(typeface);
    font.setHeight(height);
    return font;
}
static juce::Font square(float height = 8.0f)
{
    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Square_ttf, BinaryData::Square_ttfSize);
    auto font = juce::Font(typeface);
    font.setHeight(height);
    return font;
}
static juce::Font bebasNeue(float height = 8.0f)
{
    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::BebasNeueRegular_otf, BinaryData::BebasNeueRegular_otfSize);
    auto font = juce::Font(typeface);
    font.setHeight(height);
    return font;
}
static juce::Font futuraMediumItalic(float size)
{
    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Futura_Medium_Italic_otf, BinaryData::Futura_Medium_Italic_otfSize);
    auto font = juce::Font(typeface);
    font.setHeight(size);
    return font;
}
static juce::Font robotoLight(float size)
{
    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf, BinaryData::RobotoLight_ttfSize);
    auto font = juce::Font(typeface);
    font.setHeight(size);
    return font;
}
static juce::Font robotoLightItalic(float size)
{
    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::RobotoLightItalic_ttf, BinaryData::RobotoLightItalic_ttfSize);
    auto font = juce::Font(typeface);
    font.setHeight(size);
    return font;
}

};

