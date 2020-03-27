/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define Vt 0.025f // thermal voltage
#define Vtx2 Vt*2.0f

//==============================================================================
MoogLadderFilterAudioProcessor::MoogLadderFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    gainParameter.ID = "gain";
    gainParameter.name = "Gain";
    gainParameter.min = 0.0f;
    gainParameter.max = 1.0f;
    gainParameter.defaultValue = 0.5f;
    addParameter (gain = new AudioParameterFloat (gainParameter.ID, gainParameter.name, gainParameter.min, gainParameter.max, gainParameter.defaultValue));
    
    fcParameter.ID = "fc";
    fcParameter.name = "Cutoff Frequency";
    fcParameter.min = 20.0f;
    fcParameter.max = 20000.0f;
    fcParameter.defaultValue = 20000.0f;
    addParameter (fc = new AudioParameterFloat(fcParameter.ID, fcParameter.name, fcParameter.min, fcParameter.max, fcParameter.defaultValue));
    
    resonanceParameter.ID = "resonance";
    resonanceParameter.name = "Resonance";
    resonanceParameter.min = 0.0f;
    resonanceParameter.max = 10.0f;
    resonanceParameter.defaultValue = 0.0f;
    addParameter (resonance = new AudioParameterFloat(resonanceParameter.ID, resonanceParameter.name, resonanceParameter.min, resonanceParameter.max, resonanceParameter.defaultValue));

    // initialize values for the ladder filter
    setG();
    for (int i = 0; i < 6; ++i)
    {
        for (int channel = 0; channel < 2; ++channel)
        {
            y[channel][i] = 0.0f;
            yprev[channel][i] = 0.0f;
            if (i < 3)
            {
                W[channel][i] = 0.0f;
                Wprev[channel][i] = 0.0f;
                
            }
            
        }
    }
    // initialize scopeData with zeros to avoid reading erroneous data, if no audio input is initially flowing in
    zeromem (scopeData, sizeof (scopeData));
}

MoogLadderFilterAudioProcessor::~MoogLadderFilterAudioProcessor()
{
}

//==============================================================================
const String MoogLadderFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MoogLadderFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MoogLadderFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MoogLadderFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MoogLadderFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MoogLadderFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MoogLadderFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MoogLadderFilterAudioProcessor::setCurrentProgram (int index)
{
}

const String MoogLadderFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void MoogLadderFilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MoogLadderFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MoogLadderFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MoogLadderFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MoogLadderFilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
     // In case we have more outputs than inputs, this code clears any output
     // channels that didn't contain input data, (because these aren't
     // guaranteed to be empty - they may contain garbage).
     // This is here to avoid people getting screaming feedback
     // when they first compile a plugin, but obviously you don't need to keep
     // this code if your algorithm always overwrites all the output channels.
     int numSamples = buffer.getNumSamples();
     
     for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
         buffer.clear (i, 0, numSamples);

     // get write access to the audio buffer for sample by sample processing
    float** channelData = buffer.getArrayOfWritePointers();
     for (int i = 0; i < numSamples; ++i)
     {
         for (int channel = 0; channel < totalNumInputChannels; ++channel)
         {
             // process the Moog ladder filter
             float Vtx2xg = Vt*2*g;
             for (int m = 0; m < 2; ++m)
             {
                 // the first stage of the Moog ladder filter
                 float theta = (channelData[channel][i])-4.0f*(*resonance)*yprev[channel][4]/(Vtx2);
                 y[channel][0] = yprev[channel][0] + Vtx2xg*((float)tanh(theta)-Wprev[channel][0]);
                 
                 // the next two stages of the filter
                 for (int k = 0; k < 3; ++k)
                 {
                     theta = y[channel][k]/(Vtx2);
                     W[channel][k] = (float)tanh(theta);
                     if (k!=2) // the final stage is calculated differently
                     {
                         y[channel][k+1] = yprev[channel][k+1] + Vtx2xg*(W[channel][k]-Wprev[channel][k+1]);
                     }
                 }
                 
                 // the final stage of the filter
                 theta = yprev[channel][3]/Vtx2;
                 y[channel][3] = yprev[channel][3] + Vtx2xg*(W[channel][2]-(float)tanh(theta));
                 
                 // adjust the output phase
                 y[channel][4] = (y[channel][3] + yprev[channel][3])*0.5f;
                 
                 // update the unit delays
                 for (int j = 0; j < 3; ++j)
                 {
                     yprev[channel][j] = y[channel][j];
                     Wprev[channel][j] = W[channel][j];
                 }
                 yprev[channel][3] = y[channel][3];
                 yprev[channel][4] = y[channel][4];
             }
             channelData[channel][i] = y[channel][4];
             if (channel == 0)
             {
                 // audio data from channel 0 will be processed in the spectrum analyzer
                 pushNextSampleIntoFifo(*gain*300.0f*channelData[channel][i]);
             }
         }
     }
    buffer.applyGain(*gain*300.0f);
}

//==============================================================================
bool MoogLadderFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MoogLadderFilterAudioProcessor::createEditor()
{
    return new MoogLadderFilterAudioProcessorEditor (*this);
}

//==============================================================================
void MoogLadderFilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MoogLadderFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

float MoogLadderFilterAudioProcessor::setG()
{
    return g = 1.0f-(float)exp(-M_PI_2*(*fc)/getSampleRate());
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MoogLadderFilterAudioProcessor();
}

void MoogLadderFilterAudioProcessor::pushNextSampleIntoFifo(float sample) noexcept
{
    if (fifoIndex == fftSize)
    {
        if (! nextFFTBlockReady)
        {
            zeromem (fftData, sizeof (fftData));
            memcpy (fftData, fifo, sizeof (fifo));
            nextFFTBlockReady = true;
        }
        
        fifoIndex = 0;
    }
    
    fifo[fifoIndex++] = sample;
}
