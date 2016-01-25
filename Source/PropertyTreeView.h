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
};
