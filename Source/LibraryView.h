#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "MemoryView.h"
#include "SystemView.h"
#include "RC505.h"

class LibraryView : public Component,
                    public RC505::Library::Listener {
public:
    LibraryView();
    virtual ~LibraryView();

    RC505::Library &library() { return _library; }

    // Component
    virtual void paint(Graphics &g) override;
    virtual void resized() override;
    virtual void visibilityChanged() override;

    // RC505::Library::Listener
    virtual void libraryChanged() override;

private:
    RC505::Library _library;
    TabbedComponent _tabs;
    MemoryView _memoryView;
    SystemView _systemView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LibraryView)
};
