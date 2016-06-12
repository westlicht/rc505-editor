#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

static void setupLookAndFeel()
{
    LookAndFeel &lf = LookAndFeel::getDefaultLookAndFeel();

    Colour backgroundColour = Colour(0, 0, 0);

    // TreeView
    lf.setColour(TreeView::backgroundColourId, backgroundColour);
    lf.setColour(TreeView::linesColourId, Colours::white);
    lf.setColour(TreeView::dragAndDropIndicatorColourId, Colours::blue);
    lf.setColour(TreeView::selectedItemBackgroundColourId, Colours::grey);

    // TabbedComponent
    lf.setColour(TabbedComponent::backgroundColourId, Colours::black);
    lf.setColour(TabbedComponent::outlineColourId, Colours::white);

    // TabbedButtonBar
    lf.setColour(TabbedButtonBar::tabOutlineColourId, Colours::white);
    lf.setColour(TabbedButtonBar::tabTextColourId, Colours::grey);
    lf.setColour(TabbedButtonBar::frontOutlineColourId, Colours::grey);
    lf.setColour(TabbedButtonBar::frontTextColourId, Colours::white);
    
    // TextButton
    lf.setColour(TextButton::buttonColourId, Colours::black);
    lf.setColour(TextButton::textColourOnId, Colours::white);
    lf.setColour(TextButton::textColourOffId, Colours::white);

    // TextEditor
    lf.setColour(TextEditor::backgroundColourId, Colours::black);
    lf.setColour(TextEditor::textColourId, Colours::orange);
    lf.setColour(TextEditor::highlightColourId, Colours::darkgrey);
    lf.setColour(TextEditor::highlightedTextColourId, Colours::yellow);
    lf.setColour(TextEditor::outlineColourId, Colours::grey);
    lf.setColour(TextEditor::focusedOutlineColourId, Colours::white);
    lf.setColour(TextEditor::shadowColourId, Colours::darkgrey);

    // ToggleButton
    lf.setColour(ToggleButton::textColourId, Colours::white);
    
    // Slider
    lf.setColour(Slider::backgroundColourId, Colours::black);
    lf.setColour(Slider::thumbColourId, Colours::darkgrey);
    lf.setColour(Slider::trackColourId, Colours::darkgrey);
    lf.setColour(Slider::textBoxTextColourId, Colours::white);
    lf.setColour(Slider::textBoxBackgroundColourId, Colours::black);
    lf.setColour(Slider::textBoxHighlightColourId, Colours::darkgrey);
    lf.setColour(Slider::textBoxOutlineColourId, Colours::grey);
    
    // Label
    lf.setColour(Label::backgroundColourId, Colours::black);
    lf.setColour(Label::textColourId, Colours::white);

    // ProgressBar
    lf.setColour(ProgressBar::backgroundColourId, Colours::black);
    lf.setColour(ProgressBar::foregroundColourId, Colours::white);

    // ResizableWindow
    lf.setColour(ResizableWindow::backgroundColourId, Colours::black);
    
}
