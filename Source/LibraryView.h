#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PatchTreeView.h"
#include "PatchView.h"
#include "RC505.h"

class LibraryView : public Component, public RC505::Library::Listener, public DragAndDropContainer, PatchTreeView::Listener {
public:
    LibraryView(RC505::Library &library);
    virtual ~LibraryView();

    void paint(Graphics &g) override;
    void resized() override;

    void patchSelected(RC505::Patch *patch) override;

    // RC505::Library::Listener
    virtual void beforeLibraryLoaded() override;
    virtual void afterLibraryLoaded() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryView)
    
    RC505::Library &_library;
    PatchTreeView _patchTreeView;
    PatchView _patchView;
};
