#include "CustomLookAndFeel.h"
#include "JuceHeader.h"
#include "PlayButton.h"
#include "WaveformView.h"

std::unique_ptr<CustomLookAndFeel> CustomLookAndFeel::_instance;

CustomLookAndFeel::CustomLookAndFeel()
{
    _matrixTypeface = Typeface::createSystemTypefaceFor(BinaryData::Matrix_ttf, BinaryData::Matrix_ttfSize);
    _matrixFont = Font(_matrixTypeface);

    Colour backgroundColour = Colour::greyLevel(0.1f);
    Colour textColour = Colour::greyLevel(0.9f);
    Colour borderColour = Colour::greyLevel(0.5f);
    Colour focusedBorderColor = borderColour.brighter();
    Colour widgetBackgroundColour = Colour(0xff212d55);
    Colour widgetHighlightColour = widgetBackgroundColour.brighter();

    // Main
    setColour(mainBackgroundColourId, backgroundColour);
    setColour(mainTextColourId, textColour);
    setColour(mainBorderColourId, borderColour);
    setColour(mainFocusedBorderColourId, focusedBorderColor);
    setColour(mainWidgetBackgroundColourId, widgetBackgroundColour);
    setColour(mainWidgetHighlightColourId, widgetHighlightColour);

    // AlertWindow
    setColour(AlertWindow::backgroundColourId, backgroundColour);
    setColour(AlertWindow::textColourId, textColour);
    setColour(AlertWindow::outlineColourId, borderColour);

    // ProgressBar
    setColour(ProgressBar::backgroundColourId, widgetBackgroundColour);
    setColour(ProgressBar::foregroundColourId, widgetHighlightColour);

    // PopupMenu
    setColour(PopupMenu::backgroundColourId, backgroundColour);
    setColour(PopupMenu::textColourId, textColour);
    setColour(PopupMenu::headerTextColourId, textColour);
    setColour(PopupMenu::highlightedBackgroundColourId, widgetHighlightColour);
    setColour(PopupMenu::highlightedTextColourId, textColour.brighter());

    // TabbedComponent
    setColour(TabbedComponent::backgroundColourId, backgroundColour);
    setColour(TabbedComponent::outlineColourId, borderColour);

    // TreeView
    setColour(TreeView::backgroundColourId, backgroundColour);
    setColour(TreeView::linesColourId, Colours::white);
    setColour(TreeView::dragAndDropIndicatorColourId, widgetHighlightColour);
    setColour(TreeView::selectedItemBackgroundColourId, widgetHighlightColour);

    // TextButton
    setColour(TextButton::buttonColourId, Colours::black);
    setColour(TextButton::buttonOnColourId, Colours::black);
    setColour(TextButton::textColourOffId, Colour::greyLevel(0.8f));
    setColour(TextButton::textColourOnId, Colour::greyLevel(0.8f));

    // ToggleButton
    setColour(ToggleButton::textColourId, textColour);
    setColour(ToggleButton::tickColourId, textColour);
    setColour(ToggleButton::tickDisabledColourId, textColour.darker());

    // PlayButton
    setColour(PlayButton::playColourId, Colours::lime);
    setColour(PlayButton::stopColourId, Colours::red);

    // TextEditor
    setColour(TextEditor::backgroundColourId, widgetBackgroundColour);
    setColour(TextEditor::textColourId, textColour);
    setColour(TextEditor::highlightColourId, widgetHighlightColour);
    setColour(TextEditor::highlightedTextColourId, Colours::white);
    setColour(TextEditor::outlineColourId, borderColour);
    setColour(TextEditor::focusedOutlineColourId, focusedBorderColor);
    setColour(TextEditor::shadowColourId, Colours::black);

    // CaretComponent
    setColour(CaretComponent::caretColourId, widgetHighlightColour);

    // ComboBox
    setColour(ComboBox::backgroundColourId, Colours::black);
    setColour(ComboBox::textColourId, textColour);
    setColour(ComboBox::outlineColourId, borderColour);
    setColour(ComboBox::buttonColourId, textColour);
    setColour(ComboBox::arrowColourId, textColour);

    // Slider
    setColour(Slider::backgroundColourId, Colours::black);
    setColour(Slider::thumbColourId, widgetBackgroundColour);
    setColour(Slider::trackColourId, widgetBackgroundColour);
    setColour(Slider::rotarySliderFillColourId, Colours::black);
    setColour(Slider::rotarySliderOutlineColourId, Colours::black);
    setColour(Slider::textBoxTextColourId, textColour);
    setColour(Slider::textBoxBackgroundColourId, widgetBackgroundColour);
    setColour(Slider::textBoxHighlightColourId, widgetHighlightColour);
    setColour(Slider::textBoxOutlineColourId, borderColour);

    // TabbedComponent
    setColour(TabbedComponent::backgroundColourId, Colours::black);

    // WaveformView
    setColour(WaveformView::backgroundColourId, Colours::black);
    setColour(WaveformView::borderColourId, borderColour);
    setColour(WaveformView::waveformColourId, Colours::white);
    setColour(WaveformView::cursorColourId, Colours::lime);
}

void CustomLookAndFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown)
{
    float margin = 1.5f;
    float width = button.getWidth() - 2.f * margin;
    float height = button.getHeight() - 2.f * margin;
    float radius = height * 0.5f;

    // draw background
    g.setColour(backgroundColour);
    g.fillRoundedRectangle(margin, margin, width, height, radius);

    // draw border
    Colour borderColour = findColour(mainBorderColourId);
    if (isMouseOverButton)
    {
        borderColour = borderColour.brighter(0.4f);
        if (isButtonDown)
        {
            borderColour = borderColour.brighter(0.4f);
        }
    }
    g.setColour(borderColour);
    g.drawRoundedRectangle(margin, margin, width, height, radius, 3.f);
}

void CustomLookAndFeel::drawTreeviewPlusMinusBox(Graphics& g, const Rectangle<float>& area, Colour backgroundColour, bool isOpen, bool isMouseOver)
{
    ignoreUnused(backgroundColour);

    Path p;
    p.addTriangle(0.f, 0.f, 1.f, isOpen ? 0.f : 0.5f, isOpen ? 0.5f : 0.f, 1.f);
    g.setColour(findColour(mainBorderColourId).brighter(isMouseOver ? 0.4f : 0.f));
    g.fillPath(p, p.getTransformToScaleToFit(area.reduced(2, area.getHeight() / 4), true));
}

void CustomLookAndFeel::drawTickBox(Graphics& g, Component& component, float x, float y, float w, float h, bool ticked, bool isEnabled, bool isMouseOverButton, bool isButtonDown)
{
    ignoreUnused(ticked, isEnabled, isMouseOverButton, isButtonDown);

    const float boxSize = w * 0.7f;

    g.setColour(component.findColour(mainTextColourId).withMultipliedAlpha(isEnabled ? 1.f : 0.5f));
    g.drawEllipse(x, y + (h - boxSize) * 0.5f, boxSize, boxSize, 2.f);

    if (ticked)
    {
        Path tick;
        tick.startNewSubPath(1.5f, 3.f);
        tick.lineTo(3.f, 6.f);
        tick.lineTo(6.f, 0.f);
        g.setColour(component.findColour(isEnabled ? ToggleButton::tickColourId
                                                   : ToggleButton::tickDisabledColourId));
        const AffineTransform trans(AffineTransform::scale(w / 9.f, h / 9.f).translated(x, y));
        g.strokePath(tick, PathStrokeType(2.5f), trans);
    }
}

int CustomLookAndFeel::getTabButtonBestWidth(TabBarButton&, int)
{
    return 300;
}

static Colour getTabBackgroundColour(TabBarButton& button)
{
    const Colour bkg(button.findColour(mainBackgroundColourId).contrasting(0.15f));

    if (button.isFrontTab())
    {
        return bkg.overlaidWith(Colours::yellow.withAlpha(0.5f));
    }

    return bkg;
}

void CustomLookAndFeel::drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown)
{
    Rectangle<int> activeArea(button.getActiveArea());
    activeArea = activeArea.withTrimmedBottom(1);

    const Colour bkg(getTabBackgroundColour(button));

    g.setGradientFill(ColourGradient(bkg.brighter(0.1f), 0, float(activeArea.getY()), bkg.darker(0.1f), 0, float(activeArea.getBottom()), false));
    g.fillRect(activeArea);

    g.setColour(button.findColour(mainBackgroundColourId).darker(0.3f));
    g.drawRect(activeArea);

    const float alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.f : 0.8f) : 0.3f;
    const Colour col(bkg.contrasting().withMultipliedAlpha(alpha));

    TextLayout textLayout;
    LookAndFeel_V3::createTabTextLayout(button, float(activeArea.getWidth()), float(activeArea.getHeight()), col, textLayout);

    textLayout.draw(g, button.getTextArea().toFloat());
}

void CustomLookAndFeel::drawTabbedButtonBarBackground(TabbedButtonBar& bar, Graphics& g)
{
    g.fillAll(findColour(TabbedComponent::backgroundColourId));
    g.setColour(findColour(mainBorderColourId));
    g.setOpacity(1.f);
    g.drawLine(0.f, bar.getThickness() - 0.5f, bar.getWidth(), bar.getThickness() - 0.5f);
}

void CustomLookAndFeel::drawProgressBar(Graphics& g, ProgressBar& progressBar, int width, int height, double progress, const String& textToShow)
{
    ignoreUnused(progressBar);
    g.fillAll(findColour(ProgressBar::backgroundColourId));
    g.setColour(findColour(mainBorderColourId));
    g.drawRect(0, 0, width, height);
    g.setColour(findColour(ProgressBar::foregroundColourId));
    g.fillRect(1.f, 1.f, float(jlimit(0.0, width - 2.0, progress * (width - 2.0))), float(height - 2));

    if (textToShow.isNotEmpty())
    {
        g.setColour(findColour(mainTextColourId));
        g.setFont(height * 0.6f);
        g.drawText(textToShow, 0, 0, width, height, Justification::centred, false);
    }
}

int CustomLookAndFeel::getAlertWindowButtonHeight()
{
    return 30;
}

CustomLookAndFeel& CustomLookAndFeel::instance()
{
    if (!_instance)
    {
        _instance = std::make_unique<CustomLookAndFeel>();
    }
    return *_instance;
}

void CustomLookAndFeel::free()
{
    _instance = nullptr;
}
