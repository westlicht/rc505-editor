#pragma once

#include "JuceHeader.h"

#include "RC505.h"

class PropertyTreeViewItem;

class PropertyTreeView : public TreeView
{
public:
    PropertyTreeView();
    ~PropertyTreeView();

    void setGroup(RC505::Group* group);

    void setLabelWidth(int labelWidth);
    void setValueWidth(int valueWidth);

private:
    std::unique_ptr<PropertyTreeViewItem> _root;
    int _labelWidth;
    int _valueWidth;

    friend class PropertyTreeViewItem;
    template <typename TProperty, typename TPropertyView>
    friend class PropertyTreeViewComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyTreeView)
};

class PropertySetTreeViewItem;

class PropertySetTreeView : public TreeView
{
public:
    PropertySetTreeView();
    ~PropertySetTreeView();

    void setGroup(RC505::Group* group);

private:
    std::unique_ptr<PropertySetTreeViewItem> _root;

    friend class PropertySetTreeViewItem;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertySetTreeView)
};
