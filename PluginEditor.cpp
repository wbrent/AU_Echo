/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EchoDelayAudioProcessorEditor::EchoDelayAudioProcessorEditor(EchoDelayAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);

	// these define the parameters of our slider object
	delayTime.setSliderStyle(Slider::LinearBarVertical);
	delayTime.setRange(50.0, processor.delBufSizeMs, 1.0);
	delayTime.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	delayTime.setPopupDisplayEnabled(true, false, this);
	delayTime.setTextValueSuffix(" Delay Time (ms)");
	delayTime.setValue(250.0);

	// these define the parameters of our slider object
	delRampTime.setSliderStyle(Slider::LinearBarVertical);
	delRampTime.setRange(250.0, 10000, 1.0);
	delRampTime.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	delRampTime.setPopupDisplayEnabled(true, false, this);
	delRampTime.setTextValueSuffix(" Delay Ramp Time (ms)");
	delRampTime.setValue(500);

	feedbackPercent.setSliderStyle(Slider::LinearBarVertical);
	feedbackPercent.setRange(0.0, 99.00, 1.0);
	feedbackPercent.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	feedbackPercent.setPopupDisplayEnabled(true, false, this);
	feedbackPercent.setTextValueSuffix(" Feedback (%)");
	feedbackPercent.setValue(70.0);

	// this function adds the slider to the editor
	addAndMakeVisible(&delayTime);
	addAndMakeVisible(&delRampTime);
	addAndMakeVisible(&feedbackPercent);

	// add the listener to the slider
	delayTime.addListener(this);
	delRampTime.addListener(this);
	feedbackPercent.addListener(this);
}

EchoDelayAudioProcessorEditor::~EchoDelayAudioProcessorEditor()
{
}

void EchoDelayAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	processor.delTimeMsTarget = delayTime.getValue();
	processor.delTimeMsRampTime = delRampTime.getValue() / 1000.0;
	processor.delFdbk = feedbackPercent.getValue() / 100.0;
	processor.sliderValChangeFlag = true;

	//processor.delTimeSec = processor.delTimeMs / 1000.0f;
	//processor.delTimeSamps = roundf(processor.delTimeSec * processor.storedSampleRate);
	//processor.doubleDelTimeSec = processor.delTimeSec * 2.0;
	//processor.doubleDelTimeSamps = roundf(processor.doubleDelTimeSec * processor.storedSampleRate);
}
//==============================================================================
void EchoDelayAudioProcessorEditor::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	g.setColour(Colours::white);
	g.setFont(15.0f);
	g.drawFittedText("Echo Delay_V1", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void EchoDelayAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	// sets the position and size of the slider with arguments (x, y, width, height)
	delayTime.setBounds(40, 30, 20, getHeight() - 60);
	delRampTime.setBounds(100, 30, 20, getHeight() - 60);
	feedbackPercent.setBounds(160, 30, 20, getHeight() - 60);
}
