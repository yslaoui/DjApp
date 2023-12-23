/*
  ==============================================================================

    DeckGUI.h
    Created: 3 Aug 2023 12:44:49pm
    Author:  Lenovo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "./DJAudioPlayer.h"
#include "./WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget, 
    public Timer
{
public:
    DeckGUI(DJAudioPlayer* _djAudioPlayer, 
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(Button* button) override;
    void sliderValueChanged(Slider* slider) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    void timerCallback() override;
    void loadWave(juce::URL audioURL);

private:
    DJAudioPlayer* djAudioPlayer;

    WaveformDisplay waveformDisplay;
    
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::FileChooser fChooser{"Select a file..."};

    juce::Label trackTitleLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
