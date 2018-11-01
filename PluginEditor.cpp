/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Echo1AudioProcessorEditor::Echo1AudioProcessorEditor(Echo1AudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);

	// these define the parameters of our slider object
	delayTime.setSliderStyle(Slider::LinearBarVertical);
	delayTime.setRange(50.0, 2000.00, 50.0);
	delayTime.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	delayTime.setPopupDisplayEnabled(true, false, this);
	delayTime.setTextValueSuffix(" Delay Time (ms)");
	delayTime.setValue(1.0);

	feedbackPercent.setSliderStyle(Slider::LinearBarVertical);
	feedbackPercent.setRange(0.0, 99.00, 1.0);
	feedbackPercent.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	feedbackPercent.setPopupDisplayEnabled(true, false, this);
	feedbackPercent.setTextValueSuffix(" Feedback (%)");
	feedbackPercent.setValue(1.0);
	
	// this function adds the slider to the editor
	addAndMakeVisible(&delayTime);

	addAndMakeVisible(&feedbackPercent);

	// add the listener to the slider
	delayTime.addListener(this);
	
	feedbackPercent.addListener(this);
}

Echo1AudioProcessorEditor::~Echo1AudioProcessorEditor()
{
}

void Echo1AudioProcessorEditor::sliderValueChanged(Slider* slider)
{
/*
	// why doesn't this work?

	for(int i = 0; i < processor.storedNumChannels; i++)
		processor.delTime[i] = delayTime.getValue();
*/
	processor.delTime = delayTime.getValue() / 1000.0f;
	processor.delFdbk = feedbackPercent.getValue()/100.0f;
	
}
//==============================================================================
void Echo1AudioProcessorEditor::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	g.setColour(Colours::white);
	g.setFont(15.0f);
	g.drawFittedText("Echo Delay_V1", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void Echo1AudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	// sets the position and size of the slider with arguments (x, y, width, height)
	delayTime.setBounds(40, 30, 20, getHeight() - 60);

	feedbackPercent.setBounds(100, 30, 20, getHeight() - 60);
}
