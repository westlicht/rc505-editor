#include "../JuceLibraryCode/JuceHeader.h"
#include "TooltipPanel.h"

TooltipPanel::TooltipPanel()
{
    startTimer(50);
}

TooltipPanel::~TooltipPanel()
{
}

void TooltipPanel::paint(Graphics &g)
{
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.drawFittedText(_tooltip, getLocalBounds(), Justification::centred, 1);
}

void TooltipPanel::timerCallback()
{
    Desktop &desktop = Desktop::getInstance();
    const MouseInputSource mouseSource(desktop.getMainMouseSource());
    Component *component = mouseSource.isMouse() ? mouseSource.getComponentUnderMouse() : nullptr;
    TooltipClient *client = nullptr;
    while (component && (!client || client->getTooltip().isEmpty())) {
        client = dynamic_cast<TooltipClient *>(component);
        component = component->getParentComponent();
    }
    if (client != _client) {
        _client = client;
        _tooltip = _client ? _client->getTooltip() : "";
        repaint();
    }
}
