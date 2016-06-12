#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum ColourIds {
    mainBackgroundColourId = 0x2340000,
};

class CustomLookAndFeel : public LookAndFeel_V3 {
public:
    CustomLookAndFeel();

    virtual int getTabButtonBestWidth(TabBarButton&, int tabDepth) override;
    virtual void drawTabButton(TabBarButton &button, Graphics &, bool isMouseOver, bool isMouseDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};
