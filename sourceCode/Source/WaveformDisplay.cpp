/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 4 Aug 2023 3:21:13pm
    Author:  Lenovo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse)
    : audioThumbnail(100, formatManagerToUse, cacheToUse),
    fileLoaded(false),
    position(0)
{
    //We receive the AudioFormatManager and AudioThumbnailCache when we instanciate a waveform object in the main component
    //waveform has one private data member audioThumbnail, so it must be initialized using an initialition list
    //We call the constructor of the audioThumbnail data member directly instead of passing an argument to the initialization list as usual
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    if (fileLoaded) 
    {
        g.setColour(juce::Colours::mediumpurple);
        audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    else 
    {
        //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background
        g.fillAll(juce::Colours::darkolivegreen);
        g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

        g.setColour(juce::Colours::cornsilk);
        g.setFont(24.0f);
        g.drawText("No Track loaded ...", getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
    // notice that we send a pointer to setSource. We know it is a pointer because it uses new

}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position) 
    {
        position = pos;
        repaint();
    }
}
