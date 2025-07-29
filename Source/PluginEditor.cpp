/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingModSidechain_plus_JUCEAudioProcessorEditor::RingModSidechain_plus_JUCEAudioProcessorEditor (RingModSidechain_plus_JUCEAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), valueTreeState (vts), audioProcessor(p)
{
  DBG("vts.state: " + valueTreeState.state.toXmlString());

  //visualize the audio
  addAndMakeVisible(audioProcessor.visualizer);
  audioProcessor.visualizer.setColours(juce::Colours::gold, juce::Colours::black);
  
  std::cout << "clip" << std::endl;
  addAndMakeVisible(dial1Slider);
  dial1Attachment.reset (new SliderAttachment (valueTreeState, "clip", dial1Slider));
  dial1Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag); 
  dial1Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial1Slider.getTextBoxWidth(), dial1Slider.getTextBoxHeight());
  dial1Slider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
  dial1Slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::gold.withAlpha(0.75f));
  dial1Slider.setColour(juce::Slider::thumbColourId , juce::Colours::gold.brighter(1.5));
  dial1Slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
  dial1Slider.setColour(juce::Slider::textBoxOutlineColourId , juce::Colours::gold);

  addAndMakeVisible(label1);
  label1.setText ("clip", juce::dontSendNotification);
  label1.setJustificationType(juce::Justification::centred);
  label1.setColour(juce::Label::textColourId, juce::Colours::black);
  
  
  
  std::cout << "slopeCoe" << std::endl;
  addAndMakeVisible(dial2Slider);
  dial2Attachment.reset (new SliderAttachment (valueTreeState, "slopeCoe", dial2Slider));
  dial2Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);    
  dial2Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial2Slider.getTextBoxWidth(), dial2Slider.getTextBoxHeight());
  dial2Slider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
  dial2Slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::gold.withAlpha(0.75f));
  dial2Slider.setColour(juce::Slider::thumbColourId , juce::Colours::gold.brighter(1.5));
  dial2Slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
  dial2Slider.setColour(juce::Slider::textBoxOutlineColourId , juce::Colours::gold);  

  addAndMakeVisible(label2);
  label2.setText ("slopeCoe", juce::dontSendNotification);
  label2.setJustificationType(juce::Justification::centred);
  label2.setColour(juce::Label::textColourId, juce::Colours::black);
  

  setSize(480, 320);
}

//==============================================================================
void RingModSidechain_plus_JUCEAudioProcessorEditor::paint (juce::Graphics& g)
{
  g.fillAll(juce::Colours::gold); 
}

void RingModSidechain_plus_JUCEAudioProcessorEditor::resized()
{
  auto area = getLocalBounds();
  auto componentWidth = (area.getWidth() - 60)/2;
  auto componentHeight = (area.getHeight()- 60) / 2;
  auto padding = 20;  
  
  // Set the bounds for the visualizer
  audioProcessor.visualizer.setBounds(0, padding, area.getWidth(), componentHeight);

  dial1Slider.setBounds(padding, audioProcessor.visualizer.getBottom() + padding,  componentWidth, componentHeight);
  dial2Slider.setBounds(dial1Slider.getRight() + padding, audioProcessor.visualizer.getBottom() + padding,  componentWidth , componentHeight);

  label1.setBounds(dial1Slider.getX(), dial1Slider.getY() - 10, dial1Slider.getWidth(), dial1Slider.getTextBoxHeight() );
  label2.setBounds(dial2Slider.getX(), dial1Slider.getY() - 10, dial2Slider.getWidth(), dial2Slider.getTextBoxHeight() );
}
