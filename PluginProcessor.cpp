/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Echo1AudioProcessor::Echo1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	// this will be updated in prepareToPlay
	storedSampleRate = 44100.0;

	storedNumChannels = getTotalNumInputChannels();

	delBufSize = 5.0f;

	delTime = 0.2f;

	delFdbk = 0.7f;
}

Echo1AudioProcessor::~Echo1AudioProcessor()
{
	for (int i = 0; i < storedNumChannels; i++)
		delete[] delayBufferArray[i];

	delete[] delayBufferArray;
}

//==============================================================================
const String Echo1AudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool Echo1AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool Echo1AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool Echo1AudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double Echo1AudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int Echo1AudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int Echo1AudioProcessor::getCurrentProgram()
{
	return 0;
}

void Echo1AudioProcessor::setCurrentProgram(int index)
{
}

const String Echo1AudioProcessor::getProgramName(int index)
{
	return {};
}

void Echo1AudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void Echo1AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..

	storedSampleRate = sampleRate;

	delayBufferArray = new float*[storedNumChannels];

	for (int i = 0; i < storedNumChannels; i++)
	{
		// the delay buffer size in samples should really be a variable declared in .h
		delayBufferArray[i] = new float[round(delBufSize * storedSampleRate)];

		for (int j = 0; j < round(delBufSize * storedSampleRate); j++)
			delayBufferArray[i][j] = 0.0;
	}
}

void Echo1AudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Echo1AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
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

void Echo1AudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();
	int delBufSizeSamps = roundf(delBufSize * getSampleRate());
	int blockSize = buffer.getNumSamples();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, blockSize);

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	// Make sure to reset the state if your inner loop is processing
	// the samples and the outer loop is handling the channels.
	// Alternatively, you can process the samples with the channels
	// interleaved by keeping the same state.
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
		float thisDelTime = delTime * 2;
		float offset;

		if (channel == 0)
		{
			offset = delTime;
		}
		else
		{	
			offset = 0;
		}


		// mix the current block of input with the delayed block
		for (int j = 0; j < blockSize; j++)
			channelData[j] += delayBufferArray[channel][delBufSizeSamps - (int)roundf((thisDelTime-offset) * getSampleRate()) + j] * delFdbk;

		// push the delay buffer contents backwards by 1 block
		for (int j = 0; j < delBufSizeSamps - blockSize; j++)
			delayBufferArray[channel][j] = delayBufferArray[channel][j + blockSize];

		// write the input+delay mix to the head of the delay buffer
		for (int j = 0; j < blockSize; j++)
			delayBufferArray[channel][delBufSizeSamps - blockSize + j] = channelData[j];
	}
}

//==============================================================================
bool Echo1AudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Echo1AudioProcessor::createEditor()
{
	return new Echo1AudioProcessorEditor(*this);
}

//==============================================================================
void Echo1AudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void Echo1AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new Echo1AudioProcessor();
}
