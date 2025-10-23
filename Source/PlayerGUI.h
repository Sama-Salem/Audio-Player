#pragma once // PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"

using namespace juce;
using namespace std;

class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener
{
public:
	PlayerGUI();
	~PlayerGUI() override;
	
	void resized() override;
	void paint(juce::Graphics& g) override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();

private:
	PlayerAudio playerAudio;
	// GUI elements
	juce::TextButton loadButton{"Load File"};
	juce::TextButton restartButton{"Restart"};
	juce::TextButton stopButton{"Stop"};
	juce::TextButton playPauseButton{"play / pause"};
	juce::TextButton startButton{"start"};
	juce::TextButton endButton{"end"};
	juce::TextButton muteUnmuteButton{ "Mute / Unmute" }; 


	bool isPlaying = false;
	
	juce::Slider volumeSlider;
	std::unique_ptr <juce::FileChooser> fileChooser;
	// Event handlers
	float previousVolume = 0.5f;
	bool isMuted = false;
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};