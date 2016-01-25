#include "../JuceLibraryCode/JuceHeader.h"

#include "CustomLookAndFeel.h"
#include "RC505.h"
#include "LibraryView.h"
#include "SystemView.h"
#include "PatchView.h"
#include "AudioEngine.h"

class MainContentComponent : public AudioAppComponent {
public:
    MainContentComponent(RC505::Library &library) :
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

        // specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        _audioEngine.source().prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override
    {
        _audioEngine.source().getNextAudioBlock(bufferToFill);
    }

    void releaseResources() override
    {
        _audioEngine.source().releaseResources();
    }

    void paint(Graphics &g) override
    {
        g.fillAll (Colours::black);
    }

    void resized() override
    {
        _tabs.setSize(getWidth(), getHeight());
    }

private:
    CustomLookAndFeel _lookAndFeel;    
    TabbedComponent _tabs;
    LibraryView _libraryView;
    SystemView _systemView;
    AudioEngine &_audioEngine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

// (This function is called by the app startup code to create our main component)
Component *createMainContentComponent(RC505::Library &library) { return new MainContentComponent(library); }
