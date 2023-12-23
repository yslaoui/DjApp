/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "./DeckGUI.h"
#include "./PlaylistComponent.h"



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

   
private:

    // format manager and cach are needed for all decks in the app so we create instances in mainComponent
    // and feed them through 
   
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache{20}; // 20 is the number of files
    DJAudioPlayer player1{formatManager};
    DeckGUI deck1{&player1, formatManager, thumbnailCache};

    DJAudioPlayer player2{formatManager};
    DeckGUI deck2{&player2, formatManager, thumbnailCache};

    juce::MixerAudioSource mixersource;
    
    PlaylistComponent playlistComponent{&player1, &player2, &deck1, &deck2, formatManager, thumbnailCache };



    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
