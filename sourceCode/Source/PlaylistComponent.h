/*
  ==============================================================================

    PlaylistComponent.h
    Created: 7 Aug 2023 12:56:18pm
    Author:  Lenovo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include "./DJAudioPlayer.h"
#include "./DeckGUI.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component, 
                           public juce::TableListBoxModel, 
                           public juce::Button::Listener
{
public:
    PlaylistComponent(DJAudioPlayer* _djAudioPlayer1,
        DJAudioPlayer* _djAudioPlayer2,
        DeckGUI* _deckGui1,
        DeckGUI* _deckGui2,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    // pure virtual methods of the TableListBoxModel model
    /** Return the number of rows in the table*/
    int getNumRows() override;
    
    /** Draw the background of a row in the table*/
    void paintRowBackground(juce::Graphics&, 
                        int rowNumber, 
                        int width, 
                        int height, 
                        bool rowIsSelected) override;

    /** Draw the content of individual cells*/
    void paintCell(juce::Graphics&, 
                        int rowNumber, 
                        int columnId, 
                        int width, 
                        int height, 
                        bool rowIsSelected) override;


    /** Methods of the TableListBoxModel class, not pure virtual*/
    void cellClicked(int rowNumber, int columnId, const juce::MouseEvent&);
        
    juce::Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        juce::Component* existingComponentToUpdate) override;

    void buttonClicked(juce::Button* button) override;

private:
    juce::TableListBox tableComponent;
    DJAudioPlayer* djAudioPlayer1;
    DJAudioPlayer* djAudioPlayer2;
    DeckGUI* deckGui1;
    DeckGUI* deckGui2;
    std::vector<std::string> trackTitles;
    std::vector<juce::String> trackDurations;
    std::vector<juce::String> trackSampleRates;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
