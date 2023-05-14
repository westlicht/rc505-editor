#pragma once

#include "JuceHeader.h"
#include "PropertyTreeView.h"

class PropertySetDialog : public Component,
                          public Button::Listener
{
public:
    PropertySetDialog(RC505::Group* group);

    // Component
    virtual void paint(Graphics& g) override;
    virtual void resized() override;

    // Button::Listener
    virtual void buttonClicked(Button* button) override;

private:
    PropertySetTreeView _propertySetTreeView;
    TextButton _buttonCancel;
    TextButton _buttonOk;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertySetDialog)
};
