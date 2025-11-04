#pragma once // PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"

using namespace juce;
using namespace std;

class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener,
	public juce::Timer,
	public juce::ListBoxModel
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
	int getNumRows() override;
	void paintListBoxItem(int rowNumber,
		juce::Graphics& g,
		int width, int height,
		bool rowIsSelected) override;
	void selectedRowsChanged(int lastRowSelected) override;
	PlayerAudio& getPlayerAudio() { return playerAudio; }
	void sliderDragStarted(juce::Slider* slider) override;
	void sliderDragEnded(juce::Slider* slider) override;
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
	juce::TextButton ABloopButton{ "Set Start [A]" };
	juce::TextButton moveButton{ "Move" };
	juce::ComboBox moveDirection{ "Move_Direction" };
	juce::ComboBox moveTime{ "Move_Time" };
	juce::TextButton playlistButton{ "load playlist" };
	juce::ListBox playlistBox;
	juce::Label infoLabel;
	juce::Label timelabel;
	juce::Label positionLabel;
	juce::Label positionTimeLabel;
	juce::Slider progressSlider;
	juce::Slider speedSlider;
	juce::Slider volumeSlider;
	juce::Slider positionSlider;
	
	
	

	//SegmentLoop:
	bool isPlaying = false;
	bool isLooping = false;
	double pointA = 0.0;
	double pointB = 0.0;
	bool isAset = false;
	bool isBset = false;
	bool isABLooping = false;
	bool isDraggingPosition = false;


	std::unique_ptr <juce::FileChooser> fileChooser;
	void updateMetadataDisplay(const juce::File& file);
	std::vector<juce::File> playlistFiles;
	int currentTrackIndex = -1;

	// Event handlers
	float previousVolume = 0.5f;
	bool isMuted = false;
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;
	void timerCallback() override;
	/*bool isPlaying = false; */
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};