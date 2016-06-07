#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "RC505.h"

class PropertyTreeViewItem;

class PropertyTreeView : public TreeView {
public:
    PropertyTreeView();
    ~PropertyTreeView();

    void setGroup(RC505::Group *group);

private:
    ScopedPointer<PropertyTreeViewItem> _root;

    friend class PropertyTreeViewItem;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyTreeView)
};

class PropertySetTreeViewItem;

class PropertySetTreeView : public TreeView {
public:
    PropertySetTreeView();
    ~PropertySetTreeView();

    void setGroup(RC505::Group *group);

private:
    ScopedPointer<PropertySetTreeViewItem> _root;

    friend class PropertySetTreeViewItem;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertySetTreeView)
};
