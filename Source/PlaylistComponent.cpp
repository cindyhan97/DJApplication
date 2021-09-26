/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 1 Feb 2021 12:21:31am
    Author:  49811

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"


//==============================================================================

PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2,
    AudioFormatManager& formatManagerToUse, WaveformDisplay* _waveformDisplay1,
    WaveformDisplay* _waveformDisplay2) :player1(_player1),player2(_player2),waveformDisplay1(_waveformDisplay1),
                                                                               waveformDisplay2(_waveformDisplay2)
//PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    tableComponent.getHeader().addColumn("Play List",1,400);
    tableComponent.getHeader().addColumn("load",2,100);
    tableComponent.getHeader().addColumn("deck1",3,100);
    tableComponent.getHeader().addColumn("deck2",4,100);
    tableComponent.getHeader().addColumn("delete",5,100);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchingTextBox);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(loadButton);

    saveButton.addListener(this);
    loadButton.addListener(this);


    searchingTextBox.setTextToShowWhenEmpty("search your music", juce::Colours::burlywood);
    searchingTextBox.onReturnKey = [this] {searching(searchingTextBox.getText()); };
    tableComponent.setMultipleSelectionEnabled(true);
    illustration();

    getLookAndFeel().setColour(TextEditor::textColourId,Colour::fromRGB(255,204,153));
    getLookAndFeel().setColour(TextEditor::backgroundColourId,Colour::fromRGB(86,65,99));

    tableComponent.setColour(ListBox::backgroundColourId, Colour::fromRGB(218, 229, 245));
}

PlaylistComponent::~PlaylistComponent()
{

}

void PlaylistComponent::paint (juce::Graphics& g)
{
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll(Colour::fromRGB(193, 161, 202));

    //g.setColour (juce::Colours::darkblue);
/*    g.drawRect (getLocalBounds(), 1);*/   // draw an outline around the component

    g.setColour (juce::Colours::orange);
    g.setFont (15.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    saveButton.setBounds(0, 0, getWidth() / 3, getHeight() / 9);
    loadButton.setBounds(2*getWidth() / 3, 0, getWidth() / 3, getHeight() / 9);
    searchingTextBox.setBounds(getWidth() / 3, 0, getWidth() /3 , getHeight() / 9);
    tableComponent.setBounds(0, getHeight() / 9, getWidth(), 8* getHeight() /9);
}
int PlaylistComponent::getNumRows() {
    return trackTitlesNames.size();
}
void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber,
    int width, int height, bool rowIsSelected) 
{
    if (rowIsSelected) {
        g.fillAll(Colour::fromRGB(154,216,223));
    }
    else {
        g.fillAll(Colour::fromRGB(193, 161, 202));
    }
}

void PlaylistComponent::paintCell(Graphics& g, int rowNumber,
                                  int columnId, int width, int height,
                                  bool rowIsSelected) 
{
    g.drawText(trackTitlesNames[rowNumber], 2,
        0,width - 4, height, 
        Justification::centredLeft, true);
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId, bool isRowSelected,
                                                       Component* existingComponentToUpdate) 
{
    if (columnId == 2) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "LOAD" };
            String rowid{ std::to_string(rowNumber) };
            String colid{ std::to_string(columnId) };
            btn->setComponentID(colid + rowid);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "To Deck1" };
            String rowid{ std::to_string(rowNumber) };
            String colid{ std::to_string(columnId) };
            btn->setComponentID(colid + rowid);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "To Deck2" };
            String rowid{ std::to_string(rowNumber) };
            String colid{ std::to_string(columnId) };
            btn->setComponentID(colid + rowid);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 5) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "Delete" };
            String rowid{ std::to_string(rowNumber) };
            String colid{ std::to_string(columnId) };
            btn->setComponentID(colid + rowid);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;

}

void PlaylistComponent::buttonClicked(Button* button) 
{
    if (button == &saveButton) {
        auto now = std::time(0);
        std::string dt = ctime(&now);
        tm* ltm = localtime(&now);
        std::string yr = std::to_string(1900 + ltm->tm_year);
        std::string mon = std::to_string(1 + ltm->tm_mon);
        std::string mday = std::to_string(ltm->tm_mday);
        std::string hs = std::to_string(ltm->tm_hour);
        std::string mins = std::to_string(ltm->tm_min);
        std::string secs = std::to_string(ltm->tm_sec); 
        DBG("save button clicked, "+dt);
        playlistSave.open("playlist_"+ yr+"_"+mon+"_"+mday+"_"+hs+"_"+mins+"_"+secs+".txt",std::ios::out | std::ios::app);
        //collect the time and output in the txt name

        for (int i = 0; i < trackTitles.size();i++) {
        urljuce = trackTitles[i].toString(false);
        urlstd = urljuce.toStdString();
        //turn the URL into std::string
        filenamestd = trackTitlesNames[i].toStdString();
        //turn filename into std::string
        playlistSave << urlstd << ","<< filenamestd <<std::endl;
        }
    } 
    if (button == &loadButton) {
        DBG("load button clicked");
        trackTitles.clear();
        trackTitlesNames.clear();
        FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            std::ifstream playlistLoad{ chooser.getResult().getFileName().toStdString() };
            std::string line;
            std::vector<std::string> twoData;
            if (playlistLoad.is_open()){
               while (std::getline(playlistLoad, line)){
                   try {
                       twoData = tokenise(line, ',');
                       trackTitles.push_back(URL{ twoData[0] });
                       trackTitlesNames.push_back(twoData[1]);//convert std::string into juce::string   
                   }
                   catch (const std::exception& e){
                       DBG("CSVReader::readCSV bad data");
                   }
               }// end of while
            }
        }
        tableComponent.updateContent();
    }
    if (button != &saveButton && button != &loadButton) {
        std::string idStr = button->getComponentID().toStdString();
        int id = std::stoi(idStr);
        int trackIndex = std::stoi(idStr.substr(1));
        if (idStr.at(0) == '2') {
            FileChooser chooser{ "Select a file..." };
            if (chooser.browseForFileToOpen()) {
                trackTitlesNames[trackIndex] = chooser.getResult().getFileNameWithoutExtension();
                trackTitles[trackIndex] = chooser.getURLResult();
                tableComponent.updateContent();
                //DBG("now the track "+ idStr.substr(1) +" is "+ chooser.getResult().getFileNameWithoutExtension() );
            }
        }
        if (idStr.at(0) == '3') {
            player1->loadURL(URL{ trackTitles[trackIndex] });//the trackURL index is started from 0
            waveformDisplay1->loadURL(URL{ trackTitles[trackIndex] });
        }
        if (idStr.at(0) == '4') {
            player2->loadURL(URL{ trackTitles[trackIndex] });
            waveformDisplay2->loadURL(URL{ trackTitles[trackIndex] });
        }
        if (idStr.at(0) == '5') {
            if (trackIndex <= trackTitles.size() - 1 && trackIndex <= trackTitlesNames.size() - 1) {
                trackTitles.erase(trackTitles.begin() + trackIndex);
                trackTitlesNames.erase(trackTitlesNames.begin() + trackIndex);
                tableComponent.updateContent();
            }
        }
    }
    
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files) {
    return true;
}
void PlaylistComponent::filesDropped(const StringArray& files, int x, int y) {
    if (files.size() >= 1)
    {
        for (int i = 0; i < files.size(); i++) {
            trackTitlesNames.push_back(File{ files[i] } .getFileNameWithoutExtension());//the trackName index is started from 1
            trackTitles.push_back(URL{ File{files[i]} });
            juce::String urljucet = URL{ File{files[i]} }.toString(false);
            std::string urlstdt = urljuce.toStdString();
            DBG("file URL:" + urlstdt);
        }
        tableComponent.updateContent();
    }
}

void PlaylistComponent::searching(String inputtext) {
    if (inputtext == "") {
        tableComponent.deselectAllRows();
    }
    else {
        for (int i = 0; i < trackTitlesNames.size(); i++) {
            if (trackTitlesNames[i].contains(inputtext)) {
                tableComponent.selectRow(i, false, false);
            }
        }
    }
}

void PlaylistComponent::illustration() {
    String m;
    m<< "Drag the files into the playlist area and add the track!" << newLine;
    m << newLine;
    m << "press X at top right to continue";
    DialogWindow::LaunchOptions ops;
    auto* label = new Label();
    label->setText(m, dontSendNotification);
    label->setColour(Label::textColourId, Colours::whitesmoke);
    ops.content.setOwned(label);

    Rectangle<int> space(0, 0, 250, 150);
    ops.content->setSize(space.getWidth(), space.getHeight());
    ops.dialogTitle = "PLAY LIST INSTRUCTION";
    ops.dialogBackgroundColour = Colour::fromRGB(72,77,127);
    ops.escapeKeyTriggersCloseButton = true;
    ops.useNativeTitleBar = true;
    ops.resizable = true;

    dialogWindow = ops.launchAsync();
    if (dialogWindow != nullptr) {
        dialogWindow->centreWithSize(250, 150);
    }
}

std::vector<std::string> PlaylistComponent::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);
    return tokens;
}
