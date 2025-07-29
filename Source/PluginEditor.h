/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
class RingModSidechain_plus_JUCEAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
  RingModSidechain_plus_JUCEAudioProcessorEditor (RingModSidechain_plus_JUCEAudioProcessor&, juce::AudioProcessorValueTreeState& vts);
  ~RingModSidechain_plus_JUCEAudioProcessorEditor() override = default;

  //==============================================================================
  void paint (juce::Graphics& g) override;
  void resized() override; 
  typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

private:
  juce::AudioProcessorValueTreeState& valueTreeState; // ✅ Retained by reference
  RingModSidechain_plus_JUCEAudioProcessor& audioProcessor;
  juce::Slider dial1Slider;
  juce::Slider dial2Slider;

  juce::Label  label1;
  juce::Label  label2;


  ////Smart pointer to AudioProcessorValueTreeState::SliderAttachment
  std::unique_ptr<SliderAttachment> dial1Attachment;
  std::unique_ptr<SliderAttachment> dial2Attachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModSidechain_plus_JUCEAudioProcessorEditor)
};
