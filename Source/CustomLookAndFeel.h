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

    void drawTabButton(TabBarButton &button, Graphics &, bool isMouseOver, bool isMouseDown) override;
    void drawTabAreaBehindFrontButton(TabbedButtonBar &, Graphics &, int, int) override {}
    int getTabButtonBestWidth(TabBarButton&, int tabDepth) override;

private:
    Image backgroundTexture;
    Colour backgroundTextureBaseColour;
};
