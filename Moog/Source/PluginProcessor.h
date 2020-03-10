/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class MoogLadderFilterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    MoogLadderFilterAudioProcessor();
    ~MoogLadderFilterAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float setG();

    AudioParameterFloat* gain;
    float noteOnVel;
    
private:
    //==============================================================================
       AudioParameterFloat* fc;
       AudioParameterFloat* resonance;
       float g;
       float y[2][6];
       float yprev[2][6];
       float W[2][3];
       float Wprev[2][3];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoogLadderFilterAudioProcessor)
};
