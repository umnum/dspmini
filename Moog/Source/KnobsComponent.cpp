/*
  ==============================================================================

    KnobsComponent.cpp
    Created: 24 Mar 2020 9:26:25am
    Author:  Michael Castanieto

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KnobsComponent.h"

//==============================================================================
KnobsComponent::KnobsComponent(MoogLadderFilterAudioProcessor& p) :
// load the knob strip image
knobStrip(ImageFileFormat::loadFrom (BinaryData::moog_ladder_filter_knob_strip_png,BinaryData::moog_ladder_filter_knob_strip_pngSize)), processor(p)
{
    // set the look and feel of the knobs
    knobLookAndFeel = new KnobLookAndFeel(&knobStrip);
    
    // initialize the volume knob's parameters
    volumeKnob.setLookAndFeel(knobLookAndFeel);
    volumeKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    volumeKnob.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    volumeKnob.setComponentID(processor.gainParameter.ID);
    volumeKnob.setRange(processor.gainParameter.min, processor.gainParameter.max);
    volumeKnob.setValue(processor.gainParameter.defaultValue);
    volumeKnob.addListener(this);
    addAndMakeVisible(volumeKnob);
    
    // initialize the cutoff frequency knob's parameters
    cutoffFrequencyKnob.setLookAndFeel(knobLookAndFeel);
    cutoffFrequencyKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    cutoffFrequencyKnob.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    cutoffFrequencyKnob.setComponentID(processor.fcParameter.ID);
    cutoffFrequencyKnob.setRange(processor.fcParameter.min,processor.fcParameter.max);
    cutoffFrequencyKnob.setValue(processor.fcParameter.defaultValue);
    cutoffFrequencyKnob.addListener(this);
    addAndMakeVisible(cutoffFrequencyKnob);
    
    // initialize the filter emphasis knob's parameters
    filterEmphasisKnob.setLookAndFeel(knobLookAndFeel);
    filterEmphasisKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    filterEmphasisKnob.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filterEmphasisKnob.setComponentID(processor.resonanceParameter.ID);
    filterEmphasisKnob.setRange(processor.resonanceParameter.min, processor.resonanceParameter.max);
    filterEmphasisKnob.setValue(processor.resonanceParameter.defaultValue);
    filterEmphasisKnob.addListener(this);
    addAndMakeVisible(filterEmphasisKnob);
}

KnobsComponent::~KnobsComponent()
{
    volumeKnob.setLookAndFeel(nullptr);
    cutoffFrequencyKnob.setLookAndFeel(nullptr);
    filterEmphasisKnob.setLookAndFeel(nullptr);
    delete knobLookAndFeel;
}

void KnobsComponent::paint (Graphics& g)
{
}

void KnobsComponent::resized()
{
    // lay out the positions of the knobs within the knobs component
    auto knobs = getLocalBounds();
    knobs.removeFromTop(knobs.getHeight()*0.32);
    knobs.removeFromBottom(knobs.getHeight()*0.669);
    auto volumeKnobArea = knobs.removeFromLeft(knobs.getWidth()*0.197);
    volumeKnobArea.removeFromLeft(volumeKnobArea.getWidth()*0.15);
    volumeKnobArea.removeFromBottom(volumeKnobArea.getHeight()*0.00f);
    auto cutoffKnobArea = knobs.removeFromLeft(knobs.getWidth()*0.465);
    cutoffKnobArea.removeFromLeft(cutoffKnobArea.getWidth()*0.55);
    auto filterEmphasisKnobArea = knobs.removeFromLeft(knobs.getWidth()*0.839);
    filterEmphasisKnobArea.removeFromLeft(filterEmphasisKnobArea.getWidth()*0.535);
    volumeKnob.setBounds(volumeKnobArea);
    cutoffFrequencyKnob.setBounds(cutoffKnobArea);
    filterEmphasisKnob.setBounds(filterEmphasisKnobArea);
}

void KnobsComponent::sliderValueChanged(Slider *slider)
{
    auto value = slider->getValue();
    if (slider->getComponentID() == processor.gainParameter.ID)
    {
        value = getValueFromSkewFactor(processor.gainParameter.min, processor.gainParameter.max, value, 0.5f);
        *processor.gain = value;
    }
    if (slider->getComponentID() == processor.fcParameter.ID)
    {
        value = getValueFromSkewFactor(processor.fcParameter.min, processor.fcParameter.max, value, 0.4f);
        *processor.fc = value;
        processor.setG();
    }
    if (slider->getComponentID() == processor.resonanceParameter.ID)
    {
        *processor.resonance = value;
    }
}

float KnobsComponent::getValueFromSkewFactor(float min, float max, float value, float skewFactor)
{
    //get the normalized value
    value = (value - min)/(max - min);
    
    // calculate the proportion scale from the skew value
    float proportion = std::exp(std::log(value)/skewFactor);
    
    // get the non-normalized value from the skew factor
    value = (min + (max-min)*proportion);
    
    return value;
}
