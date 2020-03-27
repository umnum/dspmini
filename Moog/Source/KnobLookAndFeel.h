/*
  ==============================================================================

    KnobLookAndFeel.h
    Created: 24 Mar 2020 12:49:19pm
    Author:  Michael Castanieto
    Description: This class describes the look and feel of the knob
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
class KnobLookAndFeel : public LookAndFeel_V4
{
public:
    KnobLookAndFeel(Image* _knobStrip);
    ~KnobLookAndFeel();
    void drawRotarySlider (Graphics &, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider &) override;
    // draw the appropriate knob image frame from the knob value
    void drawFrame (Graphics &g, int x, int y, int width, int height, Slider &slider);
private:
    std::unique_ptr<Image> knobStrip;
    int frameCount;
    int frameSize;
};
