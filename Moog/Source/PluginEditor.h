/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class MoogAudioProcessorEditor  : public AudioProcessorEditor,
                                  public Slider::Listener,
                                  private Timer
{
public:
    enum
    {
        kParamControlHeight = 40,
        kParamLabelWidth = 80,
        kParamSliderWidth = 300
    };
    MoogAudioProcessorEditor (MoogAudioProcessor&);
    ~MoogAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider*) override;
    void sliderDragStarted (Slider*) override;
    void sliderDragEnded (Slider*) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MoogAudioProcessor& processor;
    
    void timerCallback() override;
    AudioParameterFloat* getParameterForSlider(Slider*);
    
    Label noParameterLabel;
    OwnedArray<Slider> paramSliders;
    OwnedArray<Label> paramLabels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoogAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
