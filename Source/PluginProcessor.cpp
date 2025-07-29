/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
RingModSidechain_plus_JUCEAudioProcessor::RingModSidechain_plus_JUCEAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties().withInput  ("Input",     juce::AudioChannelSet::stereo())           
                                        .withOutput ("Output",    juce::AudioChannelSet::stereo())
                                        .withInput  ("Sidechain", juce::AudioChannelSet::stereo())),
    visualizer(1), // Initialize the visualizer with 2 channels
    parameters(*this, nullptr, juce::Identifier("PARAMETERS"),
    juce::AudioProcessorValueTreeState::ParameterLayout {
      std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "clip",  1}, "Clip",
      juce::NormalisableRange<float>(0.f, 1.f, 0.01f),0.f),
      std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "slopeCoe",  1}, "DlopeCoe",
      juce::NormalisableRange<float>(0.f, 0.99f, 0.01f),0.f),
    }
  )
#endif
{
     // Add parameter listeners
    parameters.addParameterListener("clip", this);
    parameters.addParameterListener("slopeCoe", this);

    visualizer.setRepaintRate(30); // Set the repaint rate for the visualizer
    visualizer.setBufferSize(512); // Set the buffer size for the visualizer

}

//==============================================================================
const juce::String RingModSidechain_plus_JUCEAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RingModSidechain_plus_JUCEAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RingModSidechain_plus_JUCEAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RingModSidechain_plus_JUCEAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RingModSidechain_plus_JUCEAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RingModSidechain_plus_JUCEAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RingModSidechain_plus_JUCEAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RingModSidechain_plus_JUCEAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String RingModSidechain_plus_JUCEAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void RingModSidechain_plus_JUCEAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
    // NB: some hosts don't cope very well if you don't implement this method, even if you don't
    // really need to support programs.
}

//==============================================================================
void RingModSidechain_plus_JUCEAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
    // get the initial parameter values
    ClipParam = *parameters.getRawParameterValue("clip");
    SlopeCoeParam = *parameters.getRawParameterValue("slopeCoe");

    std::cout << "initialClipParam: " <<  ClipParam << std::endl;
    std::cout << "initialSlopeCoeParam: " << SlopeCoeParam << std::endl; 
    // Prepare the filter with the initial slope coefficient
    filter.setAlpha(SlopeCoeParam); 
    filter.reset(); 

    // Prepare the visualizer
    visualizer.clear();
}

void RingModSidechain_plus_JUCEAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RingModSidechain_plus_JUCEAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void RingModSidechain_plus_JUCEAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    //Separate side-chain buffers from main IO buffers
    auto mainInput = getBusBuffer(buffer, true, 0);      // input bus0（stereo）
    auto sidechain = getBusBuffer(buffer, true, 1);      // input bus1（mono）
    auto output    = getBusBuffer(buffer, false, 0);     // output bus0

    int numSamples = buffer.getNumSamples();
    int numMainInputChannels = mainInput.getNumChannels();

    float scaledClipParam = juce::jmap(ClipParam, 0.0f, 1.0f, 1.0f, 0.1f); 

    for (int sample = 0; sample < numSamples; ++sample)
    {
        // side chain signal 
        float scSample = sidechain.getReadPointer(0)[sample];

        //onepole filter
        float filteredSample = filter.processSample(scSample);

        //clamping
        float clampedSample = juce::jlimit(-1.0f, 1.0f, filteredSample);

        //abs
        float absSample = std::abs(clampedSample);

        //pow
        float powSample = std::pow(absSample, scaledClipParam);

        for (int ch = 0; ch < numMainInputChannels; ++ch)
        {
            float inputSample = mainInput.getReadPointer(ch)[sample];
            //ring modulation
            float ringModSample = inputSample * powSample;

            //output the processed sample
            float outputSample = juce::jlimit(-1.0f, 1.0f, inputSample + scSample - ringModSample);

            output.getWritePointer(ch)[sample] = outputSample;
        }
    }
    // Update the visualizer with the processed audio
    visualizer.pushBuffer(output);
}

//==============================================================================
bool RingModSidechain_plus_JUCEAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RingModSidechain_plus_JUCEAudioProcessor::createEditor()
{
    return new RingModSidechain_plus_JUCEAudioProcessorEditor (*this,  parameters);
}

//==============================================================================
void RingModSidechain_plus_JUCEAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void RingModSidechain_plus_JUCEAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

void RingModSidechain_plus_JUCEAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{

    if (parameterID == "clip")
    {
        std::cout << "ClipParam changed to: " << newValue << std::endl;
        ClipParam = newValue;
    }
    else if (parameterID == "slopeCoe")
    {
        std::cout << "SlopeCoeParam changed to: " << newValue << std::endl;
        SlopeCoeParam = newValue;
        filter.setAlpha(SlopeCoeParam); 
    }

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RingModSidechain_plus_JUCEAudioProcessor();
}
