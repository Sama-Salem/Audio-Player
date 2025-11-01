#pragma once // PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"

using namespace juce;
using namespace std;

class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener,
	public juce::Timer

{
public:
	PlayerGUI();
	~PlayerGUI() override;
	
	void resized() override;
	void paint(juce::Graphics& g) override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();
	void moveAudioPosition();
	void setSpeed(double ratio);

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
	juce::TextButton loopButton {" Loop "};
	juce::TextButton ABloopButton{ "Segment_Loop" };
	juce::TextButton moveButton{ "Move" };
	juce::ComboBox moveDirection{ "Move_Direction" };
	juce::ComboBox moveTime{ "Move_Time" };
	juce::Label infoLabel;
	juce::Label timelabel;
	juce::Slider progressSlider;
	/*juce::Slider volumeSlider;
	juce::Slider positionSlider;*/
	


	bool isPlaying = false;
	bool isLooping = false;
	double pointA = 0.0;
	double pointB = 0.0;
	bool isAset = false;
	bool isBset = false;
	bool isABLooping = false;

	juce::Slider speedSlider;
	juce::Slider volumeSlider;
	std::unique_ptr <juce::FileChooser> fileChooser;
	void updateMetadataDisplay(const juce::File& file);
	// Event handlers
	float previousVolume = 0.5f;
	bool isMuted = false;
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;
	void timerCallback() override;
	/*bool isPlaying = false; */
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};