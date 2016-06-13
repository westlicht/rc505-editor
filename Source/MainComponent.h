#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "RC505.h"
#include "LibraryView.h"
#include "AudioEngine.h"
#include "MountedVolumeListChangeDetector.h"
#include "TooltipPanel.h"

class MainMultiDocumentPanel;

class MainComponent : public AudioAppComponent,
                      public ApplicationCommandTarget,
                      public MountedVolumeListChangeDetector {
public:
    MainComponent();
    ~MainComponent();

    void newLibrary();
    void openLibrary();
    void saveLibrary();
    void saveLibraryAs();
    void closeLibrary();

    bool allowQuit();

    // Component
    virtual void paint(Graphics &g) override;
    virtual void resized() override;

    // AudioAppComponent
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    virtual void releaseResources() override;

    // ApplicationCommandTarget
    virtual ApplicationCommandTarget *getNextCommandTarget() override;
    virtual void getAllCommands(Array<CommandID> &commands) override;
    virtual void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) override;
    virtual bool perform(const InvocationInfo &info) override;

    // MountedVolumeListChangeDetector
    virtual void mountedVolumeListChanged() override;

private:
    void openLibrary(const File &path);
    void saveLibrary(RC505::Library &library, const File &path);
    bool allowDiscardChanges(RC505::Library &library);
    LibraryView *activeLibraryView();
    void iterateLibraryViews(std::function<void(LibraryView *)> handler);

    AudioEngine &_audioEngine;
    ScopedPointer<MainMultiDocumentPanel> _multiDocumentPanel;
    TooltipPanel _tooltipPanel;
    int _newLibraryIndex = 1;
    Atomic<int> _savingInProgress;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
