#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "CustomLookAndFeel.h"
#include "RC505.h"
#include "LibraryView.h"
#include "SystemView.h"
#include "PatchView.h"
#include "AudioEngine.h"

class MainComponent : public AudioAppComponent {
public:
    MainComponent(RC505::Library &library);
    ~MainComponent();

    LibraryView &libraryView() { return _libraryView; }
    SystemView &systemView() { return _systemView; }
    AudioEngine &audioEngine() { return _audioEngine; }

    // AudioAppComponent
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    virtual void releaseResources() override;

    // Component
    virtual void paint(Graphics &g) override;
    virtual void resized() override;

private:
    CustomLookAndFeel _lookAndFeel;
    TabbedComponent _tabs;
    LibraryView _libraryView;
    SystemView _systemView;
    AudioEngine &_audioEngine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
