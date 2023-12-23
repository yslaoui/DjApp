/*
  ==============================================================================

    DeckGUI.cpp
    Created: 3 Aug 2023 12:44:49pm
    Author:  Lenovo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "./DJAudioPlayer.h"


//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _djAudioPlayer, 
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse)
    : djAudioPlayer(_djAudioPlayer),
    waveformDisplay(formatManagerToUse, cacheToUse)
{
    
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    playButton.setColour(TextButton::buttonColourId, Colours::forestgreen);
    stopButton.setColour(TextButton::buttonColourId, Colours::darkred);
    loadButton.setColour(TextButton::buttonColourId, Colours::darkslateblue);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);

    addAndMakeVisible(trackTitleLabel);
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    posSlider.addListener(this);
    speedSlider.addListener(this);

    volSlider.setSliderStyle(Slider::Rotary);
    speedSlider.setSliderStyle(Slider::Rotary);
    
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    juce::Font newFont(24.0f, juce::Font::bold);
    trackTitleLabel.setFont(newFont);
    trackTitleLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);




    startTimer(200); // frame updates every 200ms ie 5 frames per second

    DBG(djAudioPlayer->currentState);
   
    
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::darkkhaki);
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    float rowH = getHeight() / 9;
    int colNumber = 7;
    float buttonSize = getWidth() / colNumber;
    float buttonPadding = getWidth() / colNumber;
    float buttonTopMargin = getWidth() / (2 * colNumber);

    playButton.setBounds(buttonPadding, rowH, buttonSize, rowH);
    stopButton.setBounds(3* buttonPadding, rowH, buttonSize, rowH);
    loadButton.setBounds(5 * buttonPadding, rowH, buttonSize, rowH);
    volSlider.setBounds(0, rowH * 2, getWidth()/2, 3*rowH);
    speedSlider.setBounds(3 * buttonPadding, rowH * 2, getWidth()/2, 3*rowH);
    posSlider.setBounds(0, rowH * 5, getWidth(), rowH);
    trackTitleLabel.setBounds(0, rowH * 6, getWidth() / 2, rowH/2);
    waveformDisplay.setBounds(0, rowH * 7, getWidth(), rowH * 2);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton) 
    {
        DBG(djAudioPlayer->currentState);

        if ((djAudioPlayer->currentState == DJAudioPlayer::TransportState::Stopped)
         || (djAudioPlayer->currentState == DJAudioPlayer::TransportState::Paused))
        {
            // Track is not playing
            djAudioPlayer->changeState(DJAudioPlayer::TransportState::Starting);
            djAudioPlayer->changeState(DJAudioPlayer::TransportState::Playing);
            playButton.setButtonText("PAUSE");
        }
        
        else if ((djAudioPlayer->currentState == DJAudioPlayer::TransportState::Playing))
        {
            // Track playing
            djAudioPlayer->changeState(DJAudioPlayer::TransportState::Pausing);
            djAudioPlayer->changeState(DJAudioPlayer::TransportState::Paused);
            playButton.setButtonText("PLAY");
        }
            
        DBG(djAudioPlayer->currentState);

    }

    if (button == &stopButton) 
    {
        //djAudioPlayer->stop();
        DBG(djAudioPlayer->currentState);
        if (djAudioPlayer->currentState == DJAudioPlayer::TransportState::Paused)
            djAudioPlayer->changeState(DJAudioPlayer::TransportState::Stopped);
        else
            djAudioPlayer->changeState(DJAudioPlayer::TransportState::Stopping);
            djAudioPlayer->changeState(DJAudioPlayer::TransportState::Stopped);
            playButton.setButtonText("PLAY");
        DBG(djAudioPlayer->currentState);
    }

    if (button == &loadButton) 
    {
        DBG(djAudioPlayer->currentState);
        auto fileChooserFlags =
            juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                auto chosenFile = chooser.getResult();
                djAudioPlayer->loadURL(juce::URL{ chosenFile });
                // load the file in the waveform display as well. Note that waveForm display is not a pointer so we use dot
                loadWave(URL{ chooser.getResult() });
            });
        DBG(djAudioPlayer->currentState);

    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        djAudioPlayer->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        djAudioPlayer->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        djAudioPlayer->setPositionRelative(slider->getValue());
    }

}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{   
    DBG("DeckGUI::filesDropped");

    //if (files.size() == 1) 
    //{
    //    djAudioPlayer->loadURL(juce::URL{ juce::File{files[0]} });
    //}

    for (const juce::String filename : files) 
    {
        djAudioPlayer->loadURL(juce::URL{juce::File{filename}});
        waveformDisplay.loadURL(juce::URL{juce::File{filename}});
    }
    
        
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
    
    trackTitleLabel.setFont(juce::Font(16.0f));
    trackTitleLabel.setJustificationType(juce::Justification::topLeft);
    juce::String loadedFileName = djAudioPlayer->getLoadedFileName();
    trackTitleLabel.setText(loadedFileName, juce::dontSendNotification);
    
}

void DeckGUI::loadWave(juce::URL audioURL)
{
    waveformDisplay.loadURL(audioURL);
}


/*
void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y) {
    std::cout << "DeckGUI::filesDropped" << std::endl;
    for (const auto& filename : files)
        player->loadURL(juce::URL{juce::File{filename}});
}

*/
