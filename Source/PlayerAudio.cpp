#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
	transportSource.addChangeListener(this);
}
PlayerAudio::~PlayerAudio() {
	transportSource.removeChangeListener(this);
}
void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill); 
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}
bool PlayerAudio::loadFile(const juce::File& file) {
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            // 🔑 Disconnect old source first
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            // Create new reader source
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            // Attach safely
            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);
            transportSource.start();
        }
    }
    return false;
}
void PlayerAudio::start() {
    if(transportSource.getCurrentPosition()>=transportSource.getLengthInSeconds())
		transportSource.setPosition(0.0);
    transportSource.start();
}
void PlayerAudio::stop() {
    transportSource.stop();
}
void PlayerAudio::setGain(float gain) {
    transportSource.setGain(gain);
}
void PlayerAudio::setSpeed(double speed) {
        if (readerSource != nullptr)
        {   bool wasPlaying = transportSource.isPlaying();

            double currentPosition = transportSource.getCurrentPosition();
            transportSource.stop();
            transportSource.setSource(nullptr);
            transportSource.setSource(readerSource.get(), 0, nullptr, currentSampleRate*speed);
            transportSource.setPosition(currentPosition);
            if (wasPlaying)
                transportSource.start();
        }
    
}
void PlayerAudio::setPosition(double pos) {
    transportSource.setPosition(pos);
}
double PlayerAudio::getPosition() const {
    return transportSource.getCurrentPosition();
}
double PlayerAudio::getLength() const {
    return transportSource.getLengthInSeconds();
}
void PlayerAudio::loadPlaylist(const juce::Array<juce::File>& files)
{
    playlist = files;

    if (playlist.size() > 0)
    {
        currentIndex = 0;
        loadFile(playlist[currentIndex]);
        start();
    }
}

void PlayerAudio::playNext()
{
    if (playlist.size() == 0) return;

    currentIndex = (currentIndex + 1) % playlist.size();
    loadFile(playlist[currentIndex]);
    start();
}

void PlayerAudio::playPrevious()
{
    if (playlist.size() == 0) return;

    currentIndex = (currentIndex - 1 + playlist.size()) % playlist.size();
    loadFile(playlist[currentIndex]);
    start();
}
void PlayerAudio::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.hasStreamFinished())
        {
            playNext(); 
        }
    }
}