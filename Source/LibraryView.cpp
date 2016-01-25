#include "../JuceLibraryCode/JuceHeader.h"
#include "LibraryView.h"

LibraryView::LibraryView(RC505::Library &library) :
    _library(library),
    _patchTreeView(library)
{
    addAndMakeVisible(_patchTreeView);
    addAndMakeVisible(_patchView);

    _patchTreeView.addListener(this);

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
    const int TabsHeight = 0;
    //_libraryListBox.setBounds(0, TabsHeight, LibraryViewWidth, getHeight() - TabsHeight);
    _patchTreeView.setBounds(0, TabsHeight, LibraryViewWidth, getHeight() - TabsHeight);
    _patchView.setBounds(LibraryViewWidth, TabsHeight, getWidth() - LibraryViewWidth, getHeight() - TabsHeight);
}

void LibraryView::patchSelected(RC505::Patch *patch)
{
    _patchView.setPatch(patch);
}

void LibraryView::beforeLibraryLoaded()
{
    _patchView.setPatch(nullptr);
}

void LibraryView::afterLibraryLoaded()
{
    _patchView.setPatch(_library.patches()[0]);
}

