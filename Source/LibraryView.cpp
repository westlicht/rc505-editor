#include "../JuceLibraryCode/JuceHeader.h"
#include "LibraryView.h"

LibraryView::LibraryView() :
    _tabs(TabbedButtonBar::TabsAtTop),
    _memoryView(_library),
    _systemView(_library)
{
    _library.addListener(this);

    setName(_library.name());
    addAndMakeVisible(_tabs);
    
    _tabs.addTab("Memory", Colours::white, &_memoryView, false);
    _tabs.addTab("System", Colours::white, &_systemView, false);
}

LibraryView::~LibraryView()
{
}

void LibraryView::paint(Graphics &g)
{
    g.setColour(Colours::black);
    g.fillAll();
}

void LibraryView::resized()
{
    _tabs.setBounds(0, 20, getWidth(), getHeight() - 20);
}

void LibraryView::visibilityChanged()
{
    if (!isVisible()) {
        _memoryView.patchView().stopPlaying();
    }
}

void LibraryView::libraryChanged()
{
    setName(_library.documentName());
}
