#include "MemoryView.h"
#include "CustomLookAndFeel.h"
#include "JuceHeader.h"
#include "PropertySetDialog.h"

MemoryView::MemoryView(RC505::Library& library)
    : _library(library)
    , _patchTreeView(library)
    , _patchSettingsBuffer(nullptr)
{
    // addKeyListener(this);

    addAndMakeVisible(_patchTreeView);
    addAndMakeVisible(_patchView);
    addAndMakeVisible(_copySettingsButton);
    addAndMakeVisible(_pasteSettingsButton);

    _patchTreeView.addListener(this);
    _patchTreeView.setTooltip("Click to display a patch. Select one or multiple patches and drag to rearrange.");
    _copySettingsButton.setButtonText("Copy Settings");
    _copySettingsButton.setTooltip("Copy a set of patch settings from the selected patch to the clipboard.");
    _copySettingsButton.addListener(this);
    _pasteSettingsButton.setButtonText("Paste Settings");
    _pasteSettingsButton.setTooltip("Paste a set of patch settings to the currently selected patches from the clipboard.");
    _pasteSettingsButton.addListener(this);

    _library.addListener(this);

    afterLibraryLoaded();
}

MemoryView::~MemoryView()
{
    _patchTreeView.setRootItem(nullptr);
}

void MemoryView::paint(Graphics& g)
{
    g.fillAll(findColour(mainBackgroundColourId));
    g.setColour(findColour(mainBorderColourId));
    g.drawLine(PatchTreeViewWidth + 0.5f, 0.f, PatchTreeViewWidth + 0.5f, getHeight());
}

void MemoryView::resized()
{
    _patchTreeView.setBounds(0, 0, PatchTreeViewWidth, getHeight() - 90);
    _copySettingsButton.setBounds(10, getHeight() - 80, PatchTreeViewWidth - 20, 30);
    _pasteSettingsButton.setBounds(10, getHeight() - 40, PatchTreeViewWidth - 20, 30);
    _patchView.setBounds(PatchTreeViewWidth + 1, 0, getWidth() - PatchTreeViewWidth - 1, getHeight());
}

bool MemoryView::keyPressed(const KeyPress& key)
{
    if (key.isKeyCode(KeyPress::spaceKey))
    {
        _patchView.togglePlaying();
        return true;
    }
    return false;
}

void MemoryView::patchSelected(RC505::Patch* patch)
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

void MemoryView::buttonClicked(Button* button)
{
    if (button == &_copySettingsButton)
    {
        copyPatchSettings();
    }
    if (button == &_pasteSettingsButton)
    {
        pastePatchSettings();
    }
}

void MemoryView::copyPatchSettings()
{
    // Copy settings
    if (_patchTreeView.selectedPatch())
    {
        _patchSettingsBuffer = *_patchTreeView.selectedPatch()->patchSettings();
        _patchSettingsBuffer.setSelected(false);
        PropertySetDialog dialog(&_patchSettingsBuffer);
        dialog.setSize(300, 500);
        if (DialogWindow::showModalDialog("Copy Settings", &dialog, this, Colours::white, true))
        {
            _pasteSettingsButton.setEnabled(true);
        }
    }
}

void MemoryView::pastePatchSettings()
{
    // Paste settings
    if (AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,
                                     "Paste Settings",
                                     String::formatted("Are you sure to paste the copied settings to the %d selected patch(es)?", _patchTreeView.selectedPatches().size())))
    {
        for (auto patch : _patchTreeView.selectedPatches())
        {
            patch->patchSettings()->assign(_patchSettingsBuffer, true);
        }
    }
}
