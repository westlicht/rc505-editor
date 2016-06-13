#include "../JuceLibraryCode/JuceHeader.h"
#include "MemoryView.h"
#include "PropertySetDialog.h"

MemoryView::MemoryView(RC505::Library &library) :
    _library(library),
    _patchTreeView(library),
    _patchSettingsBuffer(nullptr)
{
    addKeyListener(this);

    addAndMakeVisible(_patchTreeView);
    addAndMakeVisible(_patchView);
    addAndMakeVisible(_copySettingsButton);
    addAndMakeVisible(_pasteSettingsButton);

    _patchTreeView.addListener(this);
    _copySettingsButton.setButtonText("Copy Settings");
    _copySettingsButton.addListener(this);
    _pasteSettingsButton.setButtonText("Paste Settings");
    _pasteSettingsButton.addListener(this);

    _library.addListener(this);

    afterLibraryLoaded();
}

MemoryView::~MemoryView()
{
    _patchTreeView.setRootItem(nullptr);
}

void MemoryView::paint (Graphics &g)
{
    Component::paint(g);
}

void MemoryView::resized()
{
    const int MemoryViewWidth = 200;
    const int ButtonHeight = 30;
    _patchTreeView.setBounds(0, 0, MemoryViewWidth, getHeight() - 2 * ButtonHeight);
    _copySettingsButton.setBounds(0, getHeight() - 2 * ButtonHeight, MemoryViewWidth, ButtonHeight);
    _pasteSettingsButton.setBounds(0, getHeight() - 1 * ButtonHeight, MemoryViewWidth, ButtonHeight);
    _patchView.setBounds(MemoryViewWidth, 0, getWidth() - MemoryViewWidth, getHeight());
}

void MemoryView::patchSelected(RC505::Patch *patch)
{
    _patchView.setPatch(patch);
    _copySettingsButton.setEnabled(true);
}

void MemoryView::beforeLibraryLoaded()
{
    _patchView.setPatch(nullptr);
    _copySettingsButton.setEnabled(false);
    _pasteSettingsButton.setEnabled(false);
}

void MemoryView::afterLibraryLoaded()
{
    patchSelected(_library.patches()[0]);
}

bool MemoryView::keyPressed(const KeyPress &key, Component *originatingComponent)
{
    if (key.isKeyCode(KeyPress::spaceKey)) {
        _patchView.togglePlaying();
        return true;
    }
    return false;
}

void MemoryView::buttonClicked(Button *button)
{
    if (button == &_copySettingsButton) {
        copyPatchSettings();
    }
    if (button == &_pasteSettingsButton) {
        pastePatchSettings();
    }
}

void MemoryView::copyPatchSettings()
{
    // Copy settings
    if (_patchTreeView.selectedPatch()) {
        _patchSettingsBuffer = *_patchTreeView.selectedPatch()->patchSettings();
        _patchSettingsBuffer.setSelected(false);
        PropertySetDialog dialog(&_patchSettingsBuffer);
        dialog.setSize(300, 500);
        if (DialogWindow::showModalDialog("Copy Settings", &dialog, this, Colours::white, true)) {
            _pasteSettingsButton.setEnabled(true);
        }
    }
}

void MemoryView::pastePatchSettings()
{
    // Paste settings
    if (AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,
                                     "Paste Settings",
                                     String::formatted("Are you sure to paste the copied settings to the %d selected patch(es)?", _patchTreeView.selectedPatches().size()),
                                     String::empty, String::empty,
                                     nullptr, nullptr)) {
        for (auto patch : _patchTreeView.selectedPatches()) {
            patch->patchSettings()->assign(_patchSettingsBuffer, true);
        }
    }
}