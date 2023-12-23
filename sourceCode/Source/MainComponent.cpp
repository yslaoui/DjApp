/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 600);
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);

    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();


    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { if (granted)  setAudioChannels(2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }

    
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    mixersource.addInputSource(&player1, false);
    mixersource.addInputSource(&player2, false);

    // calls prepareToPlay() on the two players polymorphically on each of the two players
    mixersource.prepareToPlay(samplesPerBlockExpected, sampleRate);

}
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    //transportSource.getNextAudioBlock(bufferToFill);
    mixersource.getNextAudioBlock(bufferToFill);
}



void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    mixersource.removeAllInputs();
    mixersource.releaseResources();
    player1.releaseResources(); // just in case mixersource did not do the job
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
 
    deck1.setBounds(0, 0, getWidth()/2, getHeight()/2);
    deck2.setBounds(getWidth() / 2, 0 , getWidth() / 2, getHeight()/2);
    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
    
}





