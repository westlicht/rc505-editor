#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "RC505.h"

class MainWindow : public DocumentWindow,
                   public MountedVolumeListChangeDetector,
                   public DragAndDropContainer {
public:
    MainWindow(String name);
    ~MainWindow();

    virtual void closeButtonPressed() override;
    virtual void mountedVolumeListChanged() override;
    
    void openLibrary();
    void saveLibrary();
    void closeLibrary();

private:
    MainComponent *_mainComponent;
    RC505::Library _library;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
