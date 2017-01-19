/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define Vt 0.025f
#define Vtx2 Vt*2.0f


//==============================================================================
MoogAudioProcessor::MoogAudioProcessor()
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
    addParameter (gain = new AudioParameterFloat ("gain", // parameterID,
                                                  "Gain", // parameter name
                                                  0.0f,   // minimum value
                                                  1.0f,   // maximum value
                                                  1.0f));    // default value
    addParameter (fc = new AudioParameterFloat("fc", "Cutoff Frequency", 0.0f, 10000.0f, 10000.0f));
    addParameter (resonance = new AudioParameterFloat("resonance", "Resonance", 0.0f, 10.0f, 1.0f));

    // set g
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
}

MoogAudioProcessor::~MoogAudioProcessor()
{
}

//==============================================================================
const String MoogAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MoogAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MoogAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double MoogAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MoogAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MoogAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MoogAudioProcessor::setCurrentProgram (int index)
{
}

const String MoogAudioProcessor::getProgramName (int index)
{
    return String();
}

void MoogAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MoogAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MoogAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MoogAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

// This is where the Moog ladder filter is processed
void MoogAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    float maxin = 0.0f; // the 
    float maxout = 0.0f;

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    int numSamples = buffer.getNumSamples();
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int i = 0; i < numSamples; ++i)
    {
        float* channelData = buffer.getWritePointer (0);
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {

            // process the Moog ladder filter
            if (channelData[i+numSamples*channel] > maxin)
                maxin = channelData[i+numSamples*channel];
            float Vtx2xg = Vt*2*g;
            for (int m = 0; m < 2; ++m)
            {
                // the first stage of the Moog ladder filter
                float theta = (channelData[i+numSamples*channel])-4.0f*(*resonance)*yprev[channel][4]/(Vtx2);
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
                
                if (y[channel][4] > maxout)
                    maxout = y[channel][4];
                // update the unit delays
                for (int j = 0; j < 3; ++j)
                {
                    yprev[channel][j] = y[channel][j];
                    Wprev[channel][j] = W[channel][j];
                }
                yprev[channel][3] = y[channel][3];
                yprev[channel][4] = y[channel][4];
            }
            channelData[i+numSamples*channel] = y[channel][4];
        }
    }
    
    buffer.applyGain(*gain*(float)(1.0-exp(-((*fc)/1000.0)))*maxin/maxout);
}

//==============================================================================
bool MoogAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MoogAudioProcessor::createEditor()
{
    return new MoogAudioProcessorEditor (*this);
}

//==============================================================================
void MoogAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    MemoryOutputStream (destData, true).writeFloat(*gain);
    MemoryOutputStream (destData, true).writeFloat(*fc);
    MemoryOutputStream (destData, true).writeFloat(*resonance);
}

void MoogAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    *gain = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *fc = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *resonance = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
}

bool MoogAudioProcessor::isFc(Slider* param)
{
    return false;
}

void MoogAudioProcessor::setG()
{
    g = 1.0f-(float)exp(-M_PI_2*(*fc)/getSampleRate());
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MoogAudioProcessor();
}
