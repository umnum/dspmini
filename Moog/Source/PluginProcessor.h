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
    
    // set the g coefficient of the ladder filter
    float setG();
    // stores the next audio sample in a fifo buffer
    void pushNextSampleIntoFifo (float sample) noexcept;
    
    enum
    {
        fftOrder = 11,
        fftSize = 1 << fftOrder,
        scopeSize = 512 // number of data points for displaying the spectrum graphics
    };

    AudioParameterFloat* gain;
    AudioParameterFloat* fc;
    AudioParameterFloat* resonance;
    
    struct AudioParamsInit
    {
        String ID;
        String name;
        float min;
        float max;
        float defaultValue;
           
    } gainParameter, fcParameter, resonanceParameter;
    
    // stores the results of an fft calculation
    float fftData[2 * fftSize];
    // fifo buffer stores audio for fft processing
    float fifo[fftSize];
    // counts the amount of samples in the fifo
    int fifoIndex = 0;
    // a flag for showing whether the next fft block is ready to render
    bool nextFFTBlockReady = false;
    // contains the data points to be displayed
    float scopeData[scopeSize];
    
private:
    //==============================================================================
       
    float g;
    float y[2][6];
    float yprev[2][6];
    float W[2][3];
    float Wprev[2][3];
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoogLadderFilterAudioProcessor)
};
