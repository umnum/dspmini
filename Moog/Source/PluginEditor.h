/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "KnobsComponent.h"
#include "ScreenComponent.h"

//==============================================================================
/**
*/
class MoogLadderFilterAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    MoogLadderFilterAudioProcessorEditor (MoogLadderFilterAudioProcessor&);
    ~MoogLadderFilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    virtual void paintOverChildren (Graphics&) override;

   
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MoogLadderFilterAudioProcessor& processor;
    
    KnobsComponent knobsComponent;
    ScreenComponent screenComponent;
    Image backgroundImage;
    Image screenImage;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoogLadderFilterAudioProcessorEditor)
};
