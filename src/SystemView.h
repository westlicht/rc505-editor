#pragma once

#include "JuceHeader.h"

#include "PropertyTreeView.h"
#include "RC505.h"

class SystemView : public Component
{
public:
    SystemView(RC505::Library& Library);
    ~SystemView() override;

    // Component
    virtual void paint(Graphics& g) override;
    virtual void resized() override;

private:
    RC505::Library& _library;
    PropertyTreeView _propertyTreeView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SystemView)
};
