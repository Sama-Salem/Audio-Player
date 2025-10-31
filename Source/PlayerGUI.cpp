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
    g.fillAll(juce::Colours::darkgrey);
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
                }
            });
    }

    if (button == &playPauseButton)
    {
        if (isPlaying)
        {
            playerAudio.stop();
			playPauseButton.setButtonText("play");
            isPlaying = false;
            stopTimer();
        }
        else
        {
            playerAudio.start();
            playPauseButton.setButtonText("pause");
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

void PlayerGUI::timerCallback()
{
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
}
