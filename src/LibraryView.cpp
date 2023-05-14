#include "LibraryView.h"
#include "JuceHeader.h"

LibraryView::LibraryView()
    : _tabs(TabbedButtonBar::TabsAtTop)
    , _memoryView(_library)
    , _systemView(_library)
{
    _library.addListener(this);

    setName(_library.name());
    addAndMakeVisible(_tabs);

    _tabs.addTab("Memory", Colours::white, &_memoryView, false);
    _tabs.addTab("System", Colours::white, &_systemView, false);
    _tabs.getTabbedButtonBar().getTabButton(0)->setTooltip("Show patches stored in the library.");
    _tabs.getTabbedButtonBar().getTabButton(1)->setTooltip("Show system settings stored in the library.");
    _tabs.setOutline(0.f);
}

LibraryView::~LibraryView()
{
}

void LibraryView::paint(Graphics& g)
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
    if (!isVisible())
    {
        _memoryView.patchView().stopPlaying();
    }
}

void LibraryView::libraryChanged()
{
    setName(_library.documentName());
}
