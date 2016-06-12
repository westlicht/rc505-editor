#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    setColour(mainBackgroundColourId, Colour::greyLevel(0.8f));
    
    // TabbedComponent
    setColour(TabbedComponent::backgroundColourId, Colours::black);
    
//    setColour(TreeView::backgroundColourId, Colours::black);
//    setColour(TreeView::linesColourId, Colours::white);
//    setColour(TreeView::dragAndDropIndicatorColourId, Colours::blue);
//    setColour(TreeView::selectedItemBackgroundColourId, Colours::grey);
}

int CustomLookAndFeel::getTabButtonBestWidth(TabBarButton &, int) { return 300; }

static Colour getTabBackgroundColour(TabBarButton& button)
{
    const Colour bkg(button.findColour(mainBackgroundColourId).contrasting(0.15f));

    if (button.isFrontTab())
        return bkg.overlaidWith(Colours::yellow.withAlpha(0.5f));

    return bkg;
}

void CustomLookAndFeel::drawTabButton(TabBarButton &button, Graphics &g, bool isMouseOver, bool isMouseDown)
{
    const Rectangle<int> activeArea(button.getActiveArea());

    const Colour bkg(getTabBackgroundColour(button));

    g.setGradientFill(ColourGradient(bkg.brighter(0.1f), 0, float(activeArea.getY()),
                                     bkg.darker(0.1f), 0, float(activeArea.getBottom()), false));
    g.fillRect(activeArea);

    g.setColour(button.findColour(mainBackgroundColourId).darker(0.3f));
    g.drawRect(activeArea);

    const float alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.f : 0.8f) : 0.3f;
    const Colour col(bkg.contrasting().withMultipliedAlpha(alpha));

    TextLayout textLayout;
    LookAndFeel_V3::createTabTextLayout(button, float(activeArea.getWidth()), float(activeArea.getHeight()), col, textLayout);

    textLayout.draw(g, button.getTextArea().toFloat());
}
