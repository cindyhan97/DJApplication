/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                /*AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse*/
                WaveformDisplay* _waveformDisplay) : player(_player), waveformDisplay(_waveformDisplay){

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(resumeButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25);
    speedSlider.setValue(1.0);
    volSlider.setValue(1.0);

    addAndMakeVisible(volumn);
    addAndMakeVisible(position);
    addAndMakeVisible(speed);

    getLookAndFeel().setColour(Slider::thumbColourId, Colour::fromRGB(38,246,198));
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colour::fromRGB(209, 42, 86));
    getLookAndFeel().setColour(TextButton::buttonColourId, Colour::fromRGB(170,161,206));
    getLookAndFeel().setColour(TextButton::textColourOffId, Colour::fromRGB(92,113,171));
    getLookAndFeel().setColour(Slider::trackColourId,Colour::fromRGB(209, 42, 86));

    volumn.setText("Volumn", juce::dontSendNotification);
    position.setText("Position", juce::dontSendNotification);
    speed.setText("Speed", juce::dontSendNotification);

    volumn.setJustificationType(Justification::centredBottom);
    speed.setJustificationType(Justification::centredBottom);

    volumn.attachToComponent(&volSlider, false);
    position.attachToComponent(&posSlider, true);
    speed.attachToComponent(&speedSlider, false);


    //addAndMakeVisible(waveformDisplay);


    playButton.addListener(this);
    stopButton.addListener(this);
    resumeButton.addListener(this);
    //loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);


    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);

    startTimer(500);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll(Colour::fromRGB(193, 161, 202));   // clear the background

    g.setColour (Colour::fromRGB(60,255,190));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colour::fromRGB(255,255,50));
    g.setFont (14.0f);
    g.drawText ("Deck Control", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    auto labelposition = 55;//the sapce for the label
    double rowH = getHeight() / 8.0; 
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH); 
    resumeButton.setBounds(0, rowH * 2, getWidth(), rowH-10);
    volSlider.setBounds(0, rowH * 2+40, getWidth() / 2, rowH * 4);
    speedSlider.setBounds(getWidth()/2, rowH * 2+40, getWidth()/2, rowH * 4);
    posSlider.setBounds(labelposition, rowH * 7, getWidth() - 10 - labelposition, rowH);

    //waveformDisplay->setBounds(0, rowH * 5, getWidth(), rowH * 2);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        playButton.setEnabled(false);
        stopButton.setEnabled(true);
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        stopButton.setEnabled(false);
        playButton.setEnabled(true);
        player->stop();

    }
     if (button == &resumeButton) {
         stopButton.setEnabled(true);
         playButton.setEnabled(true);
         player->stop();
         posSlider.setValue(0.0);
         player->resume();
     }

    //if (button == &loadButton)
    //{
    //    FileChooser chooser{"Select a file..."};
    //    if (chooser.browseForFileToOpen())
    //    {
    //        player->loadURL(URL{chooser.getResult()});
    //        waveformDisplay->loadURL(URL{chooser.getResult()});
    //        
    //    }
    //}
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
    waveformDisplay->loadURL(URL{ File{files[0]} });
  }
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay->setPositionRelative(
            player->getPositionRelative());
    if (player->getPositionRelative() >= 0.0 && player->getPositionRelative() <= 1.0) {
        posSlider.setValue(player->getPositionRelative());
    }

}


    

