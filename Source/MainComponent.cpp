#include "MainComponent.h"
#include "PlayerGUI.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);
    setSize(800, 600);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
	mixerSource.addInputSource(&player1.getPlayerAudio(), false);
	mixerSource.addInputSource(&player2.getPlayerAudio(), false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	mixerSource.getNextAudioBlock(bufferToFill);
    //player1.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
	mixerSource.removeAllInputs();
    player1.releaseResources();
	player2.releaseResources();
}

void MainComponent::resized() 
{
	auto area = getLocalBounds().reduced(10);
	auto halfHeight = area.getHeight() / 2-10;
	player1.setBounds(area.removeFromTop(halfHeight));
	area.removeFromTop(10);
	player2.setBounds(area);
}
