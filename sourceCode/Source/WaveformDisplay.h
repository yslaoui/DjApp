/*
  ==============================================================================

    WaveformDisplay.h
    Created: 4 Aug 2023 3:21:13pm
    Author:  Lenovo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
    public::juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, 
                    juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void loadURL(juce::URL audioURL);
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /** set the relative position of the play head*/
    void setPositionRelative(double pos);

private:

    bool fileLoaded;
    juce::AudioThumbnail audioThumbnail;
    double position;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
