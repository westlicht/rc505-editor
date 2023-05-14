#pragma once

#include "JuceHeader.h"

#include "AudioEngine.h"
#include "LibraryView.h"
#include "MountedVolumeListChangeDetector.h"
#include "RC505.h"
#include "TooltipPanel.h"

class MainMultiDocumentPanel;

class MainComponent : public AudioAppComponent,
                      public ApplicationCommandTarget,
                      public MountedVolumeListChangeDetector,
                      public Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    void newLibrary();
    void openLibrary();
    void saveLibrary();
    void saveLibraryAs();
    void closeLibrary();

    bool allowQuit();

    // Component
    virtual void paint(Graphics& g) override;
    virtual void resized() override;

    // AudioAppComponent
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    virtual void releaseResources() override;

    // ApplicationCommandTarget
    virtual ApplicationCommandTarget* getNextCommandTarget() override;
    virtual void getAllCommands(Array<CommandID>& commands) override;
    virtual void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
    virtual bool perform(const InvocationInfo& info) override;

    // MountedVolumeListChangeDetector
    virtual void mountedVolumeListChanged() override;

    // Timer
    virtual void timerCallback() override;

private:
    void openLibrary(const File& path);
    void saveLibrary(RC505::Library& library, const File& path);
    bool allowDiscardChanges(RC505::Library& library);
    LibraryView* activeLibraryView();
    void iterateLibraryViews(std::function<void(LibraryView*)> handler);
    void documentsChanged();

    AudioEngine& _audioEngine;
    std::unique_ptr<MainMultiDocumentPanel> _multiDocumentPanel;
    TooltipPanel _tooltipPanel;
    int _newLibraryIndex = 1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
