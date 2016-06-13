#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class TooltipPanel : public Component,
                     public Timer {
public:
    TooltipPanel();
    virtual ~TooltipPanel();

    // Component
    virtual void paint(Graphics &g) override;

    // Timer
    virtual void timerCallback() override;

private:
    TooltipClient *_client;
    String _tooltip;
};
