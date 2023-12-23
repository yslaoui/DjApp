/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 2 Aug 2023 10:42:40am
    Author:  Lenovo

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
using namespace juce;
class DJAudioPlayer: public AudioSource , 
                     public ChangeListener
{
    public:
        DJAudioPlayer(AudioFormatManager& formatManager);
        ~DJAudioPlayer();
        void loadURL(juce::URL audioURL);
        void play();
        void stop();
        void setPosition(double posInSecs);
        void setGain(double gain);
        void setSpeed(double ratio);
        void setPositionRelative(double pos);

        // pure virtual functions of audio source class
        void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate);
        void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);
        void DJAudioPlayer::releaseResources();

        /** get the relative position of the playhead (in %) from the djAudioPlayer*/
        double getPositionRelative();

        /* getter method for the audioFileName private data member*/
        juce::String getLoadedFileName();

        

        enum TransportState
        {
            Stopped,
            Starting,
            Playing,
            Pausing,
            Paused,
            Stopping
        };

       
        TransportState currentState;
        void changeState(TransportState newState);
        void changeListenerCallback(juce::ChangeBroadcaster* source) override;



    private:

        AudioFormatManager& formatManager;
        std::unique_ptr<AudioFormatReaderSource> readerSource;
        AudioTransportSource transportSource;
        ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
        juce::String loadedFileName;

        

};