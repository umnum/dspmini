/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MoogLadderFilterAudioProcessorEditor::MoogLadderFilterAudioProcessorEditor (MoogLadderFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
      // load background images
      backgroundImage(ImageFileFormat::loadFrom(BinaryData::moog_ladder_filter_background_png, (size_t)BinaryData::moog_ladder_filter_background_pngSize)), screenImage(ImageFileFormat::loadFrom(BinaryData::moog_ladder_filter_screen_png, (size_t)BinaryData::moog_ladder_filter_screen_pngSize)), knobsComponent(p), screenComponent(p)
{
    // add the knobs and screen graphics to the main plugin editor
    addAndMakeVisible(knobsComponent);
    addAndMakeVisible(screenComponent);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(700,300);
}

MoogLadderFilterAudioProcessorEditor::~MoogLadderFilterAudioProcessorEditor()
{
}

//==============================================================================
void MoogLadderFilterAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::transparentBlack);

    // draw the screen image
    auto screenArea = getLocalBounds().toFloat();
    screenArea.removeFromTop(getHeight()*0.16f);
    screenArea.removeFromBottom(getHeight()*0.1f);
    g.drawImage(screenImage, screenArea);
    
}

void MoogLadderFilterAudioProcessorEditor::resized()
{
    // lay out the positions of the knobs component and screen component
    Rectangle<int> r = getLocalBounds();
    r.removeFromLeft(r.getWidth()/10);
    auto screenArea = r.removeFromRight(r.getWidth()/2);
    r.removeFromTop(r.getHeight()*0.122f);
    r.removeFromBottom(r.getHeight()*0.143f);
    knobsComponent.setBounds(r);
    screenArea.removeFromTop(getHeight()*0.41f);
    screenArea.removeFromBottom(getHeight()*0.259f);
    screenArea.removeFromLeft(getWidth()*0.063f);   
    screenArea.removeFromRight(getWidth()*0.095f);
    screenComponent.setBounds(screenArea);
} 

void MoogLadderFilterAudioProcessorEditor::paintOverChildren(Graphics &g)
{
    // the screen image must be behind the background image and screen graphics
    g.drawImage(backgroundImage, getLocalBounds().toFloat());
}
