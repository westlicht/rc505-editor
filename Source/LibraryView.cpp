#include "../JuceLibraryCode/JuceHeader.h"
#include "LibraryView.h"
#include "PropertySetDialog.h"

LibraryView::LibraryView(RC505::Library &library) :
    _library(library),
    _patchTreeView(library),
    _patchSettingsBuffer(nullptr)
{
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
}

LibraryView::~LibraryView()
{
    _patchTreeView.setRootItem(nullptr);
}

void LibraryView::paint (Graphics &g)
{
    Component::paint(g);
}

void LibraryView::resized()
{
    const int LibraryViewWidth = 200;
    const int ButtonHeight = 30;
    _patchTreeView.setBounds(0, 0, LibraryViewWidth, getHeight() - 2 * ButtonHeight);
    _copySettingsButton.setBounds(0, getHeight() - 2 * ButtonHeight, LibraryViewWidth, ButtonHeight);
    _pasteSettingsButton.setBounds(0, getHeight() - 1 * ButtonHeight, LibraryViewWidth, ButtonHeight);
    _patchView.setBounds(LibraryViewWidth, 0, getWidth() - LibraryViewWidth, getHeight());
}

void LibraryView::patchSelected(RC505::Patch *patch)
{
    _patchView.setPatch(patch);
    _copySettingsButton.setEnabled(true);
}

void LibraryView::beforeLibraryLoaded()
{
    _patchView.setPatch(nullptr);
    _copySettingsButton.setEnabled(false);
    _pasteSettingsButton.setEnabled(false);
}

void LibraryView::afterLibraryLoaded()
{
    patchSelected(_library.patches()[0]);
}

void LibraryView::buttonClicked(Button *button)
{
    if (button == &_copySettingsButton) {
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
    if (button == &_pasteSettingsButton) {
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
}
