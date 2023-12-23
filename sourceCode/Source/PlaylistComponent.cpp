/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 7 Aug 2023 12:56:18pm
    Author:  Lenovo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"


//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _djAudioPlayer1,
    DJAudioPlayer* _djAudioPlayer2,
    DeckGUI* _deckGui1,
    DeckGUI* _deckGui2,
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse)
    : djAudioPlayer1(_djAudioPlayer1),
    djAudioPlayer2(_djAudioPlayer2),
    deckGui1(_deckGui1),
    deckGui2(_deckGui2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    formatManagerToUse.registerBasicFormats();
    
    addAndMakeVisible(tableComponent);
    tableComponent.getHeader().addColumn("Track Title", 1, 200);
    tableComponent.getHeader().addColumn("Duration", 2, 100); 
    tableComponent.getHeader().addColumn("SampleRate", 3, 100); 
    tableComponent.getHeader().addColumn("Load deck 1", 4, 150); 
    tableComponent.getHeader().addColumn("Load deck 2", 5, 150); 
    tableComponent.setModel(this);

    juce::File directory("C:\\Documents\\uol\\uol\\course\\3_Level5S2\\oop\\code\\DJProject\\tracks");
    if (directory.exists() && directory.isDirectory()) 
    {
        juce::DirectoryIterator iterator(directory, false, "*.mp3");
        while(iterator.next()) 
        {
            juce::File file = iterator.getFile();
            std::unique_ptr<juce::AudioFormatReader> reader(formatManagerToUse.createReaderFor(file));

            // Building vector of track titles
            juce::String filename = file.getFileName();
            trackTitles.push_back(filename.toStdString());
            DBG(filename.toStdString());

            // Building vector of durations
            double durationInSeconds = reader->lengthInSamples / static_cast<double>(reader->sampleRate);
            int minutes = static_cast<int>(durationInSeconds / 60) ;
            int seconds = static_cast<int>(durationInSeconds) % 60;
            juce::String durationString = juce::String::formatted("%dm:%02ds", minutes, seconds);
            trackDurations.push_back(durationString);

            // Building vector of track sample rates
            double sampleRates = static_cast<double>(reader->sampleRate);
            juce::String sampleRateString = juce::String::formatted("%0.f", sampleRates);
            trackSampleRates.push_back(sampleRateString);

        }
    }

}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.fillAll(juce::Colours::darkgreen);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(0, 0, getWidth(), getHeight());

}

int PlaylistComponent::getNumRows()
{
    
    return trackTitles.size(); 
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected) 
    {
        g.fillAll(juce::Colours::darkgreen);
    }
    else 
    {
        g.fillAll(juce::Colours::darkolivegreen);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    
    if (columnId == 1) 
    {
        g.drawText(trackTitles[rowNumber],
            2, 1,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }

    if (columnId == 2) 
    {
        g.drawText(trackDurations[rowNumber],
            2, 1,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }

    if (columnId == 3)
    {
        g.drawText(trackSampleRates[rowNumber],
            2, 1,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }




}

void PlaylistComponent::cellClicked(int rowNumber, int columnId, const juce::MouseEvent&)
{
        
}


/*• This function will be called for every cell in the table
• If the cell is not in the column with id 1, we just return the sent Component,
which will actually be nullptr. This will only draw the table cell as usual
• If the cell is in column 1 which is the second column, we want to draw a
Button
• We check if the existingComponent coming in is nullptr. It will be nullptr
if we have not yet created the Button
• If it is nullptr, we create a TextButton and store it into existingComponent
• At the end, we return existingComponent*/

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate)
{


    if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton("Load Deck 1");
            btn->addListener(this);
            std::string id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            existingComponentToUpdate = btn;
        }
    }

    if (columnId == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton("Load Deck 2");
            btn->addListener(this);
            std::string id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            existingComponentToUpdate = btn;
        }
    }

    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{

    juce::String buttonName = button->getName();
    int id = std::stoi(button->getComponentID().toStdString());
    DBG("PlaylistComponent::buttonClicked");
    DBG(trackTitles[id]);

    // Getting the juce::URL of the clicked track
    juce::String trackTitle = trackTitles[id];
    juce::String filePath = "C:\\Documents\\uol\\uol\\course\\3_Level5S2\\oop\\code\\DJProject\\tracks\\" + trackTitle;
    juce::String urlLikeString = "file:///" + filePath.replace("\\", "/");
    juce::URL audioURL = juce::URL(urlLikeString);    
    DBG("audioURL.getFileName()");
    DBG(audioURL.getFileName());

    // Playing the track
    if (buttonName == "Load Deck 1") 
    {
        deckGui1->loadWave(audioURL);
        djAudioPlayer1->loadURL(audioURL);
        djAudioPlayer1->setPosition(0);
        //djAudioPlayer1->play();
    }

    if (buttonName == "Load Deck 2") 
    {
        deckGui2->loadWave(audioURL);
        djAudioPlayer2->loadURL(audioURL);
        djAudioPlayer2->setPosition(0);
        //djAudioPlayer2->play();
    }

}
