#include "PlayerGUI.h"

PlayerGUI::PlayerGUI() {
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton , &playPauseButton , &startButton , &endButton ,&muteUnmuteButton ,&loopButton ,&ABloopButton,&moveButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(playPauseButton);
    addAndMakeVisible(startButton);
    addAndMakeVisible(endButton);
    addAndMakeVisible(muteUnmuteButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(ABloopButton);
    addAndMakeVisible(moveButton);
    addAndMakeVisible(moveDirection);
    addAndMakeVisible(moveTime);
    addAndMakeVisible(speedSlider);
    speedSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    speedSlider.setEnabled(true);
    addAndMakeVisible(progressSlider);
    progressSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    progressSlider.setRange(0.0, 1.0);
    progressSlider.setEnabled(false);
    addAndMakeVisible(timelabel);
    timelabel.setText("0:00 / 0:00", juce::dontSendNotification);
    timelabel.setJustificationType(juce::Justification::centred);

    startTimerHz(10); 

   
    playPauseButton.addListener(this);
    startButton.addListener(this);
    endButton.addListener(this);
    muteUnmuteButton.addListener(this);
    loopButton.addListener(this);
    ABloopButton.addListener(this);
    moveButton.addListener(this);
    

    moveDirection.addItem("Forward",1);
    moveDirection.addItem("Back", 2);
    moveDirection.setSelectedId(1);
    moveTime.addItem("5s", 5);
    moveTime.addItem("10s", 10);
    moveTime.addItem("15s", 15);
    moveTime.addItem("30s", 30);
    moveTime.addItem("45s", 45);
    moveTime.addItem("60s", 60);
    moveTime.setSelectedId(5);
   
    infoLabel.setText("No file loaded", juce::dontSendNotification);
    infoLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    infoLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(infoLabel);
}
    
void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    playPauseButton.setBounds(120, y, 60, 40);
    startButton.setBounds(190, y, 60, 40);
    endButton.setBounds(260, y, 60, 40);
    restartButton.setBounds(330, y, 60, 40);
    muteUnmuteButton.setBounds(400, y, 60, 40);
    stopButton.setBounds(470, y, 60, 40);
    loopButton.setBounds(540, y, 60, 40); 
    ABloopButton.setBounds(610, y, 100, 40);
    moveButton.setBounds(710, y, 60, 40);
    moveDirection.setBounds(780, y, 60, 40);
    moveTime.setBounds(850, y, 60, 40);
	infoLabel.setBounds(20, 150, getWidth() - 40, 40);
    speedSlider.setBounds(20, 200, getWidth() - 40, 30);
    progressSlider.setBounds(20, 300, getWidth() - 40, 20);
    timelabel.setBounds(20, 330, getWidth() - 40, 20);


    /*prevButton.setBounds(340, y, 80, 40);
    nextButton.setBounds(440, y, 80, 40);*/


    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
}
PlayerGUI::~PlayerGUI() {

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
void PlayerGUI::paint(juce::Graphics& g)
{
        juce::Colour bgColour;

        if (isPlaying)
            bgColour = juce::Colours::darkgreen;  
        else
            bgColour = juce::Colours::darkgrey;   

        g.fillAll(bgColour);

        // لو عايزة شكل أشيك ممكن تضيفي:
        g.setColour(juce::Colours::white);
        g.drawRect(getLocalBounds(), 2);
}
   

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
            ABloopButton.setButtonText("Set B");
    
            
        }
        else if (!isBset)
        {
            pointB = playerAudio.getPosition();
            if (pointB > pointA)
            {
                isBset = true;
                isABLooping = true;
                ABloopButton.setButtonText("Reset A_B ");
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
        ABloopButton.setButtonText("Segment_Loop");
    }

    }

    if (button == &moveButton)
        {
            juce::Logger::outputDebugString("Move pressed - Direction: " + moveDirection.getText() +
                ", Time: " + moveTime.getText());
            moveAudioPosition();
        }
}

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

void PlayerGUI::timerCallback()
{
    if (playerAudio.getLength() > 0)
        {
            double currentPos = playerAudio.getPosition();
            double totalLength = playerAudio.getLength();
            progressSlider.setValue(currentPos / totalLength);

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
        }
    
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
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
    if (slider == &speedSlider)
        playerAudio.setSpeed((double)slider->getValue());
}