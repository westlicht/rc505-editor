#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "RC505.h"
#include "PropertyTreeView.h"

class SystemView : public Component {
public:
    SystemView(RC505::Library &Library);
    ~SystemView();

    void paint(Graphics &g);
    void resized();

private:
    RC505::Library &_library;
    PropertyTreeView _propertyTreeView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SystemView)
};
