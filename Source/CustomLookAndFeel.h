#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum ColourIds {
    mainBackgroundColourId          = 0x2340000,
};

class CustomLookAndFeel : public LookAndFeel_V3 {
public:
    CustomLookAndFeel();

#if 0
    void fillWithBackgroundTexture(Graphics&);
    static void fillWithBackgroundTexture(Component&, Graphics&);
#endif

    virtual void drawTabButton(TabBarButton &button, Graphics &, bool isMouseOver, bool isMouseDown) override;
    virtual void drawTabAreaBehindFrontButton(TabbedButtonBar &, Graphics &, int, int) override {}
    virtual int getTabButtonBestWidth(TabBarButton&, int tabDepth) override;

private:
    Image backgroundTexture;
    Colour backgroundTextureBaseColour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};
