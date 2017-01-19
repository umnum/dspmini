/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MoogAudioProcessorEditor::MoogAudioProcessorEditor (MoogAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
      noParameterLabel("noparam","No parameters available")
{
    const OwnedArray<AudioProcessorParameter>& params = p.getParameters();
    for (int i = 0; i < params.size(); ++i)
    {
        if (const AudioParameterFloat* param = dynamic_cast<AudioParameterFloat*>(params[i]))
        {
            Slider* aSlider;
           
            paramSliders.add(aSlider = new Slider (param->name));
            aSlider->setRange(param->range.start, param->range.end);
            aSlider->setSliderStyle(Slider::LinearHorizontal);
            aSlider->setValue(*param);
            aSlider->addListener(this);
            addAndMakeVisible(aSlider);
            
            Label* aLabel;
            paramLabels.add(aLabel = new Label (param->name, param->name));
            addAndMakeVisible(aLabel);
        }
    }
    
    noParameterLabel.setJustificationType(Justification::horizontallyCentred | Justification::verticallyCentred);
    noParameterLabel.setFont(noParameterLabel.getFont().withStyle(Font::italic));
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(kParamSliderWidth + kParamLabelWidth, jmax(1, kParamControlHeight*paramSliders.size()));
    
    if (paramSliders.size() == 0)
    {
        addAndMakeVisible(noParameterLabel);
    }
    else
    {
        startTimer(100);
    }

}

MoogAudioProcessorEditor::~MoogAudioProcessorEditor()
{
}

//==============================================================================
void MoogAudioProcessorEditor::paint (Graphics& g)
{
    g.setColour (Colours::white);
    g.fillRect(getLocalBounds());
}

void MoogAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    Rectangle<int> r = getLocalBounds();
    noParameterLabel.setBounds(r);
    
    for (int i = 0; i < paramSliders.size(); ++i)
    {

        Rectangle<int> paramBounds = r.removeFromTop(kParamControlHeight);
        Rectangle<int> labelBounds = paramBounds.removeFromLeft(kParamLabelWidth);
        paramLabels[i]->setBounds(labelBounds);
        paramSliders[i]->setBounds(paramBounds);
    }
}

void MoogAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (AudioParameterFloat* param = (AudioParameterFloat*)getParameterForSlider(slider))
    {
        if (param->paramID == "fc")
            processor.setG();
        *param = (float) slider->getValue();
    }
}

void MoogAudioProcessorEditor::sliderDragStarted(Slider* slider)
{
    if (AudioParameterFloat* param = (AudioParameterFloat*)getParameterForSlider(slider))
        param->beginChangeGesture();
}

void MoogAudioProcessorEditor::sliderDragEnded(Slider* slider)
{
    if (AudioParameterFloat* param = (AudioParameterFloat*)getParameterForSlider(slider))
    {
        param->endChangeGesture();
    }
}

void MoogAudioProcessorEditor::timerCallback()
{
    const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
    
    for (int i = 0; i < params.size(); ++i)
    {
        if (const AudioParameterFloat* param = dynamic_cast<AudioParameterFloat*>(params[i]))
        {
            if (i < paramSliders.size())
            {
                paramSliders[i]->setValue(*param);
            }
        }
    }
}

AudioParameterFloat* MoogAudioProcessorEditor::getParameterForSlider(Slider* slider)
{
    const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
    return dynamic_cast<AudioParameterFloat*>(params[paramSliders.indexOf(slider)]);
}