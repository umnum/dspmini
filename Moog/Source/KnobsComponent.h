/*
  ==============================================================================

    KnobsComponent.h
    Created: 24 Mar 2020 9:26:25am
    Author:  Michael Castanieto
    Description: This component is for contaning any knobs

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "KnobLookAndFeel.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class KnobsComponent    : public Component, Slider::Listener
{
public:
    KnobsComponent(MoogLadderFilterAudioProcessor&);
    ~KnobsComponent();

    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;
    

private:
    // apply the skew factor to a value within a specified range
    float getValueFromSkewFactor(float min, float max, float value, float skewFactor);
    MoogLadderFilterAudioProcessor& processor;
    KnobLookAndFeel *knobLookAndFeel;
    Image knobStrip;
    Slider volumeKnob;
    Slider cutoffFrequencyKnob;
    Slider filterEmphasisKnob;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnobsComponent)
};
