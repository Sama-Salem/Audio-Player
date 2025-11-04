#pragma once // PlayerAudio.h
#include <JuceHeader.h>

class PlayerAudio : public juce::ChangeListener, public juce::AudioSource
{
public:
	PlayerAudio();
	~PlayerAudio();
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();
	bool loadFile(const juce::File& file);
	void start();
	void stop();
	void setGain(float gain);
	void setSpeed(double speed); 
	void setPosition(double pos);
	double getPosition() const;
	double getLength() const;
	void loadPlaylist(const juce::Array<juce::File>& files);
	void playNext();
	void playPrevious();
	void changeListenerCallback(juce::ChangeBroadcaster* source);

private:
	juce::AudioFormatManager formatManager;
	std::unique_ptr <juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	std::unique_ptr<juce::ResamplingAudioSource> resamplingSource;
	juce::Array<juce::File> playlist;
	int currentIndex = -1;
	double currentSampleRate = 44100.0;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};