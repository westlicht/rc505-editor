#include "SystemView.h"
#include "JuceHeader.h"

SystemView::SystemView(RC505::Library& library)
    : _library(library)
{
    addAndMakeVisible(_propertyTreeView);
    _propertyTreeView.setGroup(_library.systemSettings());
}

SystemView::~SystemView()
{
}

void SystemView::paint(Graphics& g)
{
    g.fillAll(Colours::white);
}

void SystemView::resized()
{
    _propertyTreeView.setSize(getWidth(), getHeight());
}
