/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class EchoDelayAudioProcessor : public AudioProcessor
{
public:
	//==============================================================================
	EchoDelayAudioProcessor();
	~EchoDelayAudioProcessor();

	int sign(double x);

	// Variables which will store values from the sliders

	// float delayTimeVal;
	// float feedbackPercentVal;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

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
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;

	//==============================================================================
	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	int storedNumChannels;
	double storedSampleRate;
	int storedBlockSize;
	// this should maybe be set with a radio button (ms)
	float delTimeMsTarget;
	float delTimeMsRampRange;
	float delTimeMsRampInc;
	float delTimeMsRampTime;
	float delBufSizeMs;
	float delBufSizeSec;
	int delBufSizeSamps;
	// this should be set with a slider in ms
	float delTimeMs;
	float delTimeSec;
	int delTimeSamps;
	float doubleDelTimeSec;
	int doubleDelTimeSamps;

	// this should be set with a slider in percent (0-99)
	double delFdbk;

	float **delayBufferArray;
	float *offsetDelayBuffer;
	float *leftMix;
	
	// we need an instance of the AudioPlayHead::CurrentPositionInfo class so that we can get BPM data
	AudioPlayHead::CurrentPositionInfo currentPositionInfo;

	bool sliderValChangeFlag;
	bool delTimeMsRampFlag;

private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EchoDelayAudioProcessor)
};
