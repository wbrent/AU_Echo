/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

// test comment

//==============================================================================
/**
*/
class Echo1AudioProcessorEditor : public AudioProcessorEditor, private Slider::Listener
{
public:
	Echo1AudioProcessorEditor(Echo1AudioProcessor&);
	~Echo1AudioProcessorEditor();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

private:

	void sliderValueChanged(Slider* slider) override;

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	Echo1AudioProcessor& processor;

	Slider delayTime;
	Slider feedbackPercent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Echo1AudioProcessorEditor)
};
