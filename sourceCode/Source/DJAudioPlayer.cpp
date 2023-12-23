/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 2 Aug 2023 10:42:40am
    Author:  Lenovo

  ==============================================================================
*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) 
    : formatManager(_formatManager), 
      currentState(Stopped)  
{

}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    DBG("DJAudioPlayer::loadURL Hi");
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        loadedFileName = audioURL.getFileName();
    }
}

void DJAudioPlayer::play()
{
    DBG("DJAudioPlayer::play() hi");
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    if (posInSecs < 0 || posInSecs > transportSource.getLengthInSeconds()) {
        std::cout << "DJAudioPlayer::setPosition: warning set position " << posInSecs \
            << " greater than length " << transportSource.getLengthInSeconds() << std::endl;
        return;
    }
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setGain(double gain)
{
    transportSource.setGain(gain);

}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100.0)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 100");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr) 
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

juce::String DJAudioPlayer::getLoadedFileName()
{
    return loadedFileName;
}

void DJAudioPlayer::changeState(TransportState newState)
{
    if (currentState != newState)
    {
        currentState = newState;

        switch (currentState)
        {
        case Stopped:
            transportSource.setPosition(0.0);
            break;

        case Starting:
            transportSource.start();
            break;

        case Playing:
            break;

        case Pausing:
            transportSource.stop();
            break;

        case Paused:
            break;

        case Stopping:
            transportSource.stop();
            break;
        }
    }

}

void DJAudioPlayer::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            changeState(Playing);
        else if ((currentState == Stopping) || (currentState== Playing))
            changeState(Stopped);
        else if (currentState == Pausing)
            changeState(Paused);
    }

}



void DJAudioPlayer::setPositionRelative(double pos)
{

    if (pos < 0 || pos > 1.0)
    {
        DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

