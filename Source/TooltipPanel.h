#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class TooltipPanel : public Component,
                     public Timer {
public:
    TooltipPanel();
    virtual ~TooltipPanel();

    void setDefaultTooltip(const String &tooltip);

    // Component
    virtual void paint(Graphics &g) override;

    // Timer
    virtual void timerCallback() override;

private:
    String _defaultTooltip;
    String _tooltip;
};
