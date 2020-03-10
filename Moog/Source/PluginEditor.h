/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MoogLadderFilterAudioProcessorEditor  : public AudioProcessorEditor,
                                              public Slider::Listener{
public:
    enum
    {
        kParamControlHeight = 40,
        kParamLabelWidth = 80,
        kParamSliderWidth = 300
    };
    MoogLadderFilterAudioProcessorEditor (MoogLadderFilterAudioProcessor&);
    ~MoogLadderFilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
   
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MoogLadderFilterAudioProcessor& processor;
    
    void sliderValueChanged (Slider*) override;
    void sliderDragStarted (Slider*) override;
    void sliderDragEnded (Slider*) override;

    AudioParameterFloat* getParameterForSlider(Slider*);
    
    Label noParameterLabel;
    OwnedArray<Slider> paramSliders;
    OwnedArray<Label> paramLabels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoogLadderFilterAudioProcessorEditor)
};
