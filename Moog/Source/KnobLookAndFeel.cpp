/*
  ==============================================================================

    KnobLookAndFeel.cpp
    Created: 24 Mar 2020 12:49:19pm
    Author:  Michael Castanieto

  ==============================================================================
*/

#include "KnobLookAndFeel.h"

KnobLookAndFeel::KnobLookAndFeel(Image* _knobStrip) : knobStrip(_knobStrip), frameCount(60)
{
    int width = knobStrip->getWidth();
    frameSize = width/frameCount;
}

KnobLookAndFeel::~KnobLookAndFeel()
{
    knobStrip.release();
}

void KnobLookAndFeel::drawRotarySlider (Graphics &g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider &slider)
{
    drawFrame(g, x, y, width, height, slider);
}

void KnobLookAndFeel::drawFrame(Graphics &g, int x, int y, int width, int height, Slider &slider)
{
    const double div = slider.getMaximum()/frameCount;
    int pos = (int)(slider.getValue()/div);
    
    if (pos > 0)
    {
        pos = pos - 1;
    }
    
    g.drawImage(*knobStrip, x, y, width, height, (int)(pos*frameSize), 0, knobStrip->getWidth()/frameCount, knobStrip->getHeight());
    
}
