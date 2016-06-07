#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

MainComponent::MainComponent(RC505::Library &library) :
    _tabs(TabbedButtonBar::TabsAtTop),
    _libraryView(library),
    _systemView(library),
    _audioEngine(AudioEngine::instance())
{
    setLookAndFeel(&_lookAndFeel);
    
    addAndMakeVisible(_tabs);
    
    _tabs.addTab("Library", Colours::white, &_libraryView, false);
    _tabs.addTab("System", Colours::white, &_systemView, false);
    
    setSize(1400, 800);
    setAudioChannels(2, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    _audioEngine.source().prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    _audioEngine.source().getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    _audioEngine.source().releaseResources();
}

void MainComponent::paint(Graphics &g)
{
    g.fillAll (Colours::black);
}

void MainComponent::resized()
{
    _tabs.setSize(getWidth(), getHeight());
}
