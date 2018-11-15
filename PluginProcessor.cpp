/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EchoDelayAudioProcessor::EchoDelayAudioProcessor()
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

	delBufSize = 5.0f; // 5 seconds max delay time
	delTime = 0.25f; // 250ms initial delay time
	delFdbk = 0.7f; // 70% initial feedback
}

EchoDelayAudioProcessor::~EchoDelayAudioProcessor()
{
	for (int i = 0; i < storedNumChannels; i++)
		delete[] delayBufferArray[i];

	delete[] delayBufferArray;
	delete[] offsetDelayBuffer;
	delete[] leftMix;
}

//==============================================================================
const String EchoDelayAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool EchoDelayAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool EchoDelayAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool EchoDelayAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double EchoDelayAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int EchoDelayAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int EchoDelayAudioProcessor::getCurrentProgram()
{
	return 0;
}

void EchoDelayAudioProcessor::setCurrentProgram(int index)
{
}

const String EchoDelayAudioProcessor::getProgramName(int index)
{
	return {};
}

void EchoDelayAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void EchoDelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..

	storedSampleRate = sampleRate;

	// first, set up the offset delay buffer for the left channel
	offsetDelayBuffer = new float[round(delBufSize * storedSampleRate)];

	// fill the offset delay with silence to initialize
	for (int i = 0; i < round(delBufSize * storedSampleRate); i++)
		offsetDelayBuffer[i] = 0.0;

	// next, set up the leftMix buffer to store the offsetDelayed left  channel along with the output of the main left delay buffer
	leftMix = new float[samplesPerBlock];

	// fill the leftMix buffer with silence to initialize
	for (int i = 0; i < samplesPerBlock; i++)
		leftMix[i] = 0.0;

	// now, set up the main L/R delay buffers
	delayBufferArray = new float*[storedNumChannels];

	for (int i = 0; i < storedNumChannels; i++)
	{
		// the delay buffer size in samples should really be a variable declared in .h
		delayBufferArray[i] = new float[round(delBufSize * storedSampleRate)];

		// should make a variable for delBufSizeSamps
		for (int j = 0; j < round(delBufSize * storedSampleRate); j++)
			delayBufferArray[i][j] = 0.0;
	}
}

void EchoDelayAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.

	// WHY IN GOD'S NAME DOES IT CAUSE A CRASH TO DO DELETE THE MEMORY BUFFERS HERE?
	// THE DELETES ARE DONE IN THE DESTRUCTOR FOR NOW
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EchoDelayAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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

void EchoDelayAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
		float doubleDelTime = delTime * 2;

		switch (channel)
		{
			// for the left channel, we need to put the latest block of channelData[0] into offsetDelayBuffer
			// for the left channel, we need to mix the offsetDelayBuffer (not channelData) with the main delay buffer
			case 0:
				// push the delay buffer contents backwards by 1 block
				for (int i = 0; i < delBufSizeSamps - blockSize; i++)
					offsetDelayBuffer[i] = offsetDelayBuffer[i + blockSize];

				// write the input to the head of the delay buffer
				for (int i = 0; i < blockSize; i++)
					offsetDelayBuffer[delBufSizeSamps - blockSize + i] = channelData[i] * delFdbk;
				// mix the current block of input with the delayed block
				
				// mix the offset-delayed left channel with the main left channel delay output
				for (int i = 0; i < blockSize; i++)
					leftMix[i] = offsetDelayBuffer[delBufSizeSamps - (int)roundf(delTime*storedSampleRate) + i] + delayBufferArray[channel][delBufSizeSamps - (int)roundf(doubleDelTime*storedSampleRate) + i] * delFdbk;

				// push the delay buffer contents backwards by 1 block
				for (int j = 0; j < delBufSizeSamps - blockSize; j++)
					delayBufferArray[channel][j] = delayBufferArray[channel][j + blockSize];

				// write the input+delay mix to the head of the delay buffer
				for (int i = 0; i < blockSize; i++)
					delayBufferArray[channel][delBufSizeSamps - blockSize + i] = leftMix[i];

				// this is the output of the left channel
				for (int i = 0; i < blockSize; i++)
					channelData[i] += leftMix[i];

				break;

			default:
				// mix the current block of input with the delayed block
				for (int i = 0; i < blockSize; i++)
					channelData[i] += delayBufferArray[channel][delBufSizeSamps - (int)roundf(doubleDelTime*storedSampleRate) + i] * delFdbk;
				
				// push the delay buffer contents backwards by 1 block
				for (int i = 0; i < delBufSizeSamps - blockSize; i++)
					delayBufferArray[channel][i] = delayBufferArray[channel][i + blockSize];

				// write the input+delay mix to the head of the delay buffer
				for (int i = 0; i < blockSize; i++)
					delayBufferArray[channel][delBufSizeSamps - blockSize + i] = channelData[i];
				
				break;
		}


	}
}

//==============================================================================
bool EchoDelayAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* EchoDelayAudioProcessor::createEditor()
{
	return new EchoDelayAudioProcessorEditor(*this);
}

//==============================================================================
void EchoDelayAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void EchoDelayAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new EchoDelayAudioProcessor();
}
