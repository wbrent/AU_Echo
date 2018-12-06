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
class EchoDelayAudioProcessorEditor : public AudioProcessorEditor, private Slider::Listener
{
public:
	enum RadioButtonIds
	{
		TempoSyncButtonID = 1001
	};

	EchoDelayAudioProcessorEditor(EchoDelayAudioProcessor&);
	~EchoDelayAudioProcessorEditor();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

	void updateToggleState(Button* button, String name)
	{
		auto state = button->getToggleState();
		String stateString = state ? "ON" : "OFF";
		String selectedString = state ? " (selected)" : "";

		Logger::outputDebugString(name + " Button changed to " + stateString);
		button->setButtonText(name + selectedString);
	}

private:

	void sliderValueChanged(Slider* slider) override;

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	EchoDelayAudioProcessor& processor;

	Slider delayTime;
	Slider feedbackPercent;

	// Creating the Toggle button:

	// Label syncEnable{ {}};
	TextButton tempoSyncButton{ "Tempo Sync" };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EchoDelayAudioProcessorEditor)
};
