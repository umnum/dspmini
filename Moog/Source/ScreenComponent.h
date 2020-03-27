/*
  ==============================================================================

    ScreenComponent.h
    Created: 24 Mar 2020 4:52:39pm
    Author:  Michael Castanieto
    Drescription: This is a component class for displaying the audio spectrum graphics

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ScreenComponent : public Component, private Timer
{
public:
    ScreenComponent(MoogLadderFilterAudioProcessor&);
    ~ScreenComponent();
    
    void paint(Graphics&) override;
    void resized() override;
    
private:
    void timerCallback() override;
    // render the audio data to get the next frame of spectrum data to display
    void drawNextFrameOfSpectrum();
    // display the graphics to the spectrum data
    void drawFrame (Graphics& g);

    MoogLadderFilterAudioProcessor &processor;
                             
    dsp::WindowingFunction<float> window;
    dsp::FFT forwardFFT;

};

