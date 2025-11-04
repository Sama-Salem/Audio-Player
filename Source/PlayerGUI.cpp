#include "PlayerGUI.h"
#include "CustomLookAndFeel.h"

//Constructor:
PlayerGUI::PlayerGUI()
{
    setLookAndFeel(&customLook);
        for (auto* btn : { &loadButton, &restartButton , &stopButton , &playPauseButton ,
                           &startButton , &endButton ,&muteUnmuteButton ,&loopButton ,
                           &ABloopButton ,&moveButton })
        {
            btn->addListener(this);
            addAndMakeVisible(btn);
        }

        volumeSlider.setRange(0.0, 1.0, 0.01);
        volumeSlider.setValue(0.5);
        volumeSlider.addListener(this);
        addAndMakeVisible(volumeSlider);

        speedSlider.setRange(0.5, 2.0, 0.01);
        speedSlider.setValue(1.0);
        speedSlider.addListener(this);
        addAndMakeVisible(speedSlider);

        progressSlider.setRange(0.0, 1.0);
        progressSlider.setEnabled(false);
        addAndMakeVisible(progressSlider);

        positionSlider.setRange(0.0, 1.0);
        positionSlider.addListener(this);
        addAndMakeVisible(positionSlider);

        timelabel.setText("0:00 / 0:00", juce::dontSendNotification);
        timelabel.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(timelabel);

        addAndMakeVisible(playlistButton);
        addAndMakeVisible(playlistBox);
        playlistButton.addListener(this);
        playlistBox.setModel(this);

        moveDirection.addItem("Forward", 1);
        moveDirection.addItem("Back", 2);
        moveDirection.setSelectedId(1);
        addAndMakeVisible(moveDirection);

        moveTime.addItem("5s", 5);
        moveTime.addItem("10s", 10);
        moveTime.addItem("15s", 15);
        moveTime.addItem("30s", 30);
        moveTime.addItem("45s", 45);
        moveTime.addItem("60s", 60);
        moveTime.setSelectedId(5);
        addAndMakeVisible(moveTime);

        infoLabel.setText("No file loaded", juce::dontSendNotification);
        infoLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        infoLabel.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(infoLabel);

        startTimerHz(10);
    }
//Resize:
void PlayerGUI::resized()
{
    auto area = getLocalBounds().reduced(10);
    int buttonWidth = 100;
    int buttonHeight = 30;
    int spacing = 8;

    juce::Rectangle<int> leftArea = area.removeFromLeft(getWidth() * 0.55f);
    juce::Rectangle<int> rightArea = area;

    int columnWidth = (leftArea.getWidth() - spacing) / 2;

    juce::Rectangle<int> col1 = leftArea.removeFromLeft(columnWidth);
    leftArea.removeFromLeft(spacing);
    juce::Rectangle<int> col2 = leftArea;

    juce::Component* buttons[] = {
        &loadButton, &playPauseButton, &startButton, &endButton, &restartButton,
        &muteUnmuteButton, &stopButton, &loopButton, &ABloopButton,
        &moveButton, &moveDirection, &moveTime, &playlistButton
    };

    int buttonCount = 0;
    for (int i = 0; i < (int)(sizeof(buttons) / sizeof(buttons[0])); ++i)
    {
        juce::Component* btn = buttons[i];
        if (buttonCount % 2 == 0)
        {
            btn->setBounds(col1.removeFromTop(buttonHeight));
            col1.removeFromTop(spacing);
        }
        else
        {
            btn->setBounds(col2.removeFromTop(buttonHeight));
            col2.removeFromTop(spacing);
        }
        buttonCount++;
    }

    rightArea.removeFromTop(10);
    playlistBox.setBounds(rightArea.removeFromTop(120));
    rightArea.removeFromTop(10);
    volumeSlider.setBounds(rightArea.removeFromTop(20));
    rightArea.removeFromTop(8);
    speedSlider.setBounds(rightArea.removeFromTop(20));
    rightArea.removeFromTop(6);
    progressSlider.setBounds(rightArea.removeFromTop(20));
    rightArea.removeFromTop(8);
    timelabel.setBounds(rightArea.removeFromTop(15));
    rightArea.removeFromTop(6);
    positionSlider.setBounds(rightArea.removeFromTop(20));
    rightArea.removeFromTop(6);
    infoLabel.setBounds(rightArea.removeFromTop(25));
}


    //Paint
    void PlayerGUI::paint(juce::Graphics & g)
    {
        g.fillAll(juce::Colour::fromRGB(85, 90, 100)); 
        g.setColour(juce::Colours::white);
        g.drawRect(getLocalBounds(), 2);
    }
    PlayerGUI::~PlayerGUI()
    {
        setLookAndFeel(nullptr);
    }

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

//Buttons'conditions:

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",

            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile()) {
                    playerAudio.loadFile(file);
                    updateMetadataDisplay(file);
                }
            });
    }

    if (button == &playPauseButton)
    {
        if (isPlaying)
        {
            playerAudio.stop();
			playPauseButton.setButtonText("play");
            playPauseButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
            isPlaying = false;
            stopTimer();
        }
        else
        {
            playerAudio.start();
            playPauseButton.setButtonText("pause");
            playPauseButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
            isPlaying = true;
            startTimer(500);
		}
    }

    if (button == &startButton)
    {
        playerAudio.setPosition(0.0);
	}

    if (button == &endButton)
    {
        playerAudio.setPosition(playerAudio.getLength());
    }

    if (button == &restartButton)
    {
        playerAudio.start();
    }

    if (button == &stopButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
        stopTimer();
    }
    if (button == &muteUnmuteButton) {
        if (!isMuted) {
            previousVolume = (float)volumeSlider.getValue();
            playerAudio.setGain(0.0f);
            muteUnmuteButton.setButtonText("Unmute");
            isMuted = true;

        }
        else {
            playerAudio.setGain(previousVolume);
            muteUnmuteButton.setButtonText("Mute");
            isMuted = false;
        }
    }
    if (button == &loopButton) {
        isLooping = !isLooping;
        loopButton.setButtonText(isLooping ? "Unloop" : "Loop");
    }

    if (button == &ABloopButton)
    {
        if (!isAset)
        {
            pointA = playerAudio.getPosition();
            isAset = true;
            ABloopButton.setButtonText("Set End B");
    
            
        }
        else if (!isBset)
        {
            pointB = playerAudio.getPosition();
            if (pointB > pointA)
            {
                isBset = true;
                isABLooping = true;
                ABloopButton.setButtonText("Reset [A,B] ");
            }
            else
            {
                pointB = 0.0;
            }
        }
    else
    {
        isAset = false;
        isBset = false;
        isABLooping = false;
        pointA = 0.0;
        pointB = 0.0;
        ABloopButton.setButtonText("Set Start [A]");
    }

    }

    if (button == &moveButton)
        {
            juce::Logger::outputDebugString("Move pressed - Direction: " + moveDirection.getText() +
                ", Time: " + moveTime.getText());
            moveAudioPosition();
    }
    if (button == &playlistButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select multiple audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles |
            juce::FileBrowserComponent::canSelectMultipleItems,
            [this](const juce::FileChooser& fc)
            {
                auto files = fc.getResults(); 

                if (files.size() > 0)
                {
                    playlistFiles.clear();
                    for (int i = 0; i < files.size(); ++i)
                        playlistFiles.push_back(files.getReference(i));

                    playlistBox.updateContent();
                    playlistBox.repaint();

                    playerAudio.loadPlaylist(files);

                    currentTrackIndex = 0;
                    playlistBox.selectRow(0);
                    updateMetadataDisplay(playlistFiles[0]);
                }
            });
    }
}
//MetaData:
void PlayerGUI::updateMetadataDisplay(const juce::File& file)
{
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    if (auto* reader = formatManager.createReaderFor(file))
    {
        juce::String title = reader->metadataValues["Title"];
        juce::String artist = reader->metadataValues["Artist"];
        double duration = reader->lengthInSamples / reader->sampleRate;

        juce::String info;

        if (title.isNotEmpty() || artist.isNotEmpty())
        {
            info << "Title: " << (title.isNotEmpty() ? title : "Unknown")
                << " | Artist: " << (artist.isNotEmpty() ? artist : "Unknown")
                << " | Duration: " << juce::String(duration, 2) << " sec";
        }
        else
        {
            info << "File: " << file.getFileNameWithoutExtension()
                << " | Duration: " << juce::String(duration, 2) << " sec";
        }

        infoLabel.setText(info, juce::dontSendNotification);
        delete reader;
    }
}
//Timer:
void PlayerGUI::timerCallback()
{
    if (playerAudio.getLength() > 0)
        {
            double currentPos = playerAudio.getPosition();
            double totalLength = playerAudio.getLength();
            progressSlider.setValue(currentPos / totalLength);
            if (totalLength > 0.0 && !isDraggingPosition)
                positionSlider.setValue(currentPos / totalLength, juce::dontSendNotification);
            int currentMins = (int)(currentPos / 60);
            int currentSecs = (int)fmod(currentPos, 60);
            int totalMins = (int)(totalLength / 60);
            int totalSecs = (int)fmod(totalLength, 60);
            timelabel.setText(
                juce::String(currentMins) + ":" +
                juce::String(currentSecs).paddedLeft('0', 2) + " / " +
                juce::String(totalMins) + ":" +
                juce::String(totalSecs).paddedLeft('0', 2),
                juce::dontSendNotification);
          
            positionTimeLabel.setText(
                juce::String::formatted("%02d:%02d", currentMins, currentSecs),
                juce::dontSendNotification 
            );
        }
    //SegmentLoop:
    double currentPos = playerAudio.getPosition();

    if (isLooping && !isABLooping && currentPos>= playerAudio.getLength())
    {
        playerAudio.setPosition(0.0);
        playerAudio.start();
    }
    else
    {
        if (isABLooping && isAset &&isBset)
            if (currentPos >= pointB && pointB >pointA)
            {
                playerAudio.setPosition(pointA);
                playerAudio.start();
                
            }
    }
}
//Move:
void PlayerGUI::moveAudioPosition()
{
    double currentPos = playerAudio.getPosition();
    double moveSeconds = moveTime.getSelectedId();
    double newPos = currentPos;

    if (moveDirection.getSelectedId() == 1) 
        newPos += moveSeconds;
    else 
        newPos -= moveSeconds;

    if (newPos < 0) newPos = 0;
    if (newPos > playerAudio.getLength())
        newPos = playerAudio.getLength();
    playerAudio.setPosition(newPos);
}
//Sliders'conditions:
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
    if (slider == &speedSlider)
        playerAudio.setSpeed((double)slider->getValue());
    if (slider == &positionSlider)
    {
        double totalLength = playerAudio.getLength();
        double newPos = positionSlider.getValue() * totalLength;
        playerAudio.setPosition(newPos);
    }
}
void PlayerGUI::sliderDragStarted(juce::Slider* slider)
{
    if (slider == &positionSlider)
        isDraggingPosition = true;
}

void PlayerGUI::sliderDragEnded(juce::Slider* slider)
{
    if (slider == &positionSlider)
    {
        isDraggingPosition = false;

        double totalLength = playerAudio.getLength();
        double newPos = positionSlider.getValue() * totalLength;
        playerAudio.setPosition(newPos);
    }
}



int PlayerGUI::getNumRows()
{
    return (int)playlistFiles.size();
}
//PlaylistBox:
void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::pink);
    else if (rowNumber == currentTrackIndex)
        g.fillAll(juce::Colours::darkgreen);
    else
        g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    if (rowNumber < playlistFiles.size())
        g.drawText(playlistFiles[rowNumber].getFileName(),
            5, 0, width, height,
            juce::Justification::centredLeft);
}

void PlayerGUI::selectedRowsChanged(int lastRowSelected)
{
    if (lastRowSelected >= 0 && lastRowSelected < playlistFiles.size())
    {
        currentTrackIndex = lastRowSelected;
        playerAudio.loadFile(playlistFiles[currentTrackIndex]);
        updateMetadataDisplay(playlistFiles[currentTrackIndex]);
        playerAudio.start();
    }
}