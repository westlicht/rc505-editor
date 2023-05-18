#pragma once

#include "JuceHeader.h"

enum ColourIds
{
    mainBackgroundColourId = 0x2340000,
    mainTextColourId = 0x2340001,
    mainBorderColourId = 0x2340002,
    mainFocusedBorderColourId = 0x2340003,
    mainWidgetBackgroundColourId = 0x2340004,
    mainWidgetHighlightColourId = 0x2340005,
};

class CustomLookAndFeel : public LookAndFeel_V3
{
public:
    CustomLookAndFeel();

    const Font& matrixFont() const { return _matrixFont; }
    Font& matrixFont() { return _matrixFont; }

    virtual void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override;

    virtual void drawTreeviewPlusMinusBox(Graphics& g, const Rectangle<float>& area, Colour backgroundColour, bool isOpen, bool isMouseOver) override;
    virtual void drawTickBox(Graphics& g, Component& component, float x, float y, float w, float h, bool ticked, bool isEnabled, bool isMouseOverButton, bool isButtonDown) override;

    virtual int getTabButtonBestWidth(TabBarButton&, int tabDepth) override;
    virtual void drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override;
    virtual void drawTabbedButtonBarBackground(TabbedButtonBar& bar, Graphics& g) override;

    virtual void drawProgressBar(Graphics& g, ProgressBar& progressBar, int width, int height, double progress, const String& textToShow) override;

    virtual int getAlertWindowButtonHeight() override;

    static CustomLookAndFeel& instance();
    static void free();

private:
    static std::unique_ptr<CustomLookAndFeel> _instance;

    Typeface::Ptr _matrixTypeface;
    Font _matrixFont;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};
