/*
  ==============================================================================

    ScreenComponent.cpp
    Created: 24 Mar 2020 4:52:39pm
    Author:  Michael Castanieto

  ==============================================================================
*/

#include "ScreenComponent.h"

ScreenComponent::ScreenComponent(MoogLadderFilterAudioProcessor& p) : processor(p), forwardFFT(processor.fftOrder), window(processor.fftSize, dsp::WindowingFunction<float>::hann)
{
    // spectrum data is displayed every 40 milliseconds
    startTimer(40);
    
}

ScreenComponent::~ScreenComponent()
{
}

void ScreenComponent::paint(Graphics &g)
{
    drawFrame(g);
}

void ScreenComponent::resized()
{
    
}

void ScreenComponent::timerCallback()
{
    if (processor.nextFFTBlockReady)
    {
        drawNextFrameOfSpectrum();
        processor.nextFFTBlockReady = false;
        repaint();
    }
}

void ScreenComponent::drawNextFrameOfSpectrum()
{
    // apply the windowing function
    window.multiplyWithWindowingTable(processor.fftData, processor.fftSize);
    
    // render the fft data
    forwardFFT.performFrequencyOnlyForwardTransform(processor.fftData);
    
    auto mindB = -100.0f;
    auto maxdB = 0.0f;
    for (int i = 0; i < processor.scopeSize; ++i)
    {
        // skew the x-axis to use a logarithmic scale to better represent our frequencies
        auto skewedProportionX = 1.0f - std::exp (std::log (1.0f - i / (float) processor.scopeSize) * 0.2f);
        // calculate the level proportionally to the desired minimum and maximum decibels
        auto fftDataIndex = jlimit (0, processor.fftSize / 2, (int) (skewedProportionX * processor.fftSize / 2));
        auto level = jmap (jlimit (mindB, maxdB, Decibels::gainToDecibels (processor.fftData[fftDataIndex])
                                               - Decibels::gainToDecibels ((float) processor.fftSize)),
                           mindB, maxdB, 0.0f, 1.0f);
 
        processor.scopeData[i] = level;
    }
}

void ScreenComponent::drawFrame(Graphics &g)
{
    auto width = getLocalBounds().getWidth();
    auto height = getLocalBounds().getHeight();
    g.beginTransparencyLayer(0.6);
    Path p;
    static const DropShadow shadow(Colour(0xbb000000), 20, Point<int>(0, 0));
    p.startNewSubPath(0, getHeight());
    
    for (int i = 1; i < processor.scopeSize; ++i)
    {
        p.lineTo((float) jmap(i, 0, processor.scopeSize - 1, 0, width), (float) jmap(processor.scopeData[i], 0.0f, 1.0f, (float) height, 0.0f));
        
    }
    p.lineTo(width,height);
    p.lineTo(0,height);
    shadow.drawForPath(g, p);
    g.setGradientFill (ColourGradient (Colour(0xff00fa00), getWidth()/2, getHeight()/2,
    Colours::transparentBlack, getHeight()*1.2f, getWidth()*1.2f, true));
    g.fillPath(p);
    g.setColour(Colour(0xff64fa00));
    g.strokePath(p, PathStrokeType(1.0f, PathStrokeType::beveled, PathStrokeType::rounded));
    g.endTransparencyLayer();
}

