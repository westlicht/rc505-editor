#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PatchTreeView.h"
#include "PatchView.h"
#include "RC505.h"

class MemoryView : public Component,
                   public RC505::Library::Listener,
                   public KeyListener,
                   public DragAndDropContainer,
                   public PatchTreeView::Listener,
                   public Button::Listener {
public:
    MemoryView(RC505::Library &library);
    virtual ~MemoryView();

    PatchTreeView &patchTreeView() { return _patchTreeView; }
    PatchView &patchView() { return _patchView; }

    // Component
    virtual void paint(Graphics &g) override;
    virtual void resized() override;

    // PatchTreeView::Listener
    virtual void patchSelected(RC505::Patch *patch) override;

    // RC505::Library::Listener
    virtual void beforeLibraryLoaded() override;
    virtual void afterLibraryLoaded() override;

    // KeyListener
    virtual bool keyPressed(const KeyPress &key, Component *originatingComponent) override;

    // Button::Listener
    virtual void buttonClicked(Button *button) override;

private:
    void copyPatchSettings();
    void pastePatchSettings();
    
    RC505::Library &_library;
    PatchTreeView _patchTreeView;
    PatchView _patchView;

    TextButton _copySettingsButton;
    TextButton _pasteSettingsButton;
    RC505::PatchSettings _patchSettingsBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MemoryView)
};
