#include "PropertyTreeView.h"
#include "CustomLookAndFeel.h"
#include "PropertyView.h"

// ----------------------------------------------------------------------------
// PropertyTreeViewComponent
// ----------------------------------------------------------------------------

template <typename TProperty, typename TPropertyView>
class PropertyTreeViewComponent : public Component
{
public:
    PropertyTreeViewComponent(PropertyTreeView* treeView, TProperty* property)
        : _treeView(treeView)
        , _property(property)
        , _propertyView(property)
    {
        addAndMakeVisible(_propertyView);
    }

    virtual void resized() override
    {
        _propertyView.setBounds(_treeView->_labelWidth, 0, jmin(_treeView->_valueWidth, getWidth() - _treeView->_labelWidth), getHeight());
    }

private:
    PropertyTreeView* _treeView;
    TProperty* _property;
    TPropertyView _propertyView;
};

// ----------------------------------------------------------------------------
// PropertyTreeViewItem
// ----------------------------------------------------------------------------

class PropertyTreeViewItem : public TreeViewItem
{
public:
    PropertyTreeViewItem(RC505::Property* property)
        : _property(property)
    {
    }

    int getItemWidth() const override { return -1; }
    int getItemHeight() const override { return 20; }

    virtual String getUniqueName() const override
    {
        if (!_property)
        {
            return "unknown";
        }
        String name = _property->name();
        if (getParentItem())
        {
            name = getParentItem()->getUniqueName() + name;
        }
        return name;
    }

    virtual void itemOpennessChanged(bool isNowOpen) override
    {
        if (isNowOpen)
        {
            if (const RC505::Group* group = dynamic_cast<const RC505::Group*>(_property))
            {
                for (auto child : group->children())
                {
                    if (child->visible())
                    {
                        addSubItem(new PropertyTreeViewItem(child));
                    }
                }
            }
        }
        else
        {
            clearSubItems();
        }
    }

    virtual std::unique_ptr<Component> createItemComponent() override
    {
        PropertyTreeView* treeView = static_cast<PropertyTreeView*>(getOwnerView());
        if (RC505::BoolProperty* boolProperty = dynamic_cast<RC505::BoolProperty*>(_property))
        {
            return std::make_unique<PropertyTreeViewComponent<RC505::BoolProperty, BoolPropertyView>>(treeView, boolProperty);
        }
        else if (RC505::BitSetProperty* bitSetProperty = dynamic_cast<RC505::BitSetProperty*>(_property))
        {
            return std::make_unique<PropertyTreeViewComponent<RC505::BitSetProperty, BitSetPropertyView>>(treeView, bitSetProperty);
        }
        else if (RC505::IntProperty* intProperty = dynamic_cast<RC505::IntProperty*>(_property))
        {
            return std::make_unique<PropertyTreeViewComponent<RC505::IntProperty, IntPropertyView>>(treeView, intProperty);
        }
        else if (RC505::EnumProperty* enumProperty = dynamic_cast<RC505::EnumProperty*>(_property))
        {
            return std::make_unique<PropertyTreeViewComponent<RC505::EnumProperty, EnumPropertyView>>(treeView, enumProperty);
        }
        else if (RC505::NameProperty* nameProperty = dynamic_cast<RC505::NameProperty*>(_property))
        {
            return std::make_unique<PropertyTreeViewComponent<RC505::NameProperty, NamePropertyView>>(treeView, nameProperty);
        }
        else if (RC505::ValueProperty* valueProperty = dynamic_cast<RC505::ValueProperty*>(_property))
        {
            return std::make_unique<PropertyTreeViewComponent<RC505::ValueProperty, ValuePropertyView>>(treeView, valueProperty);
        }
        return nullptr;
    }

    virtual void paintItem(Graphics& g, int width, int height) override
    {
        auto& lookAndFeel = LookAndFeel::getDefaultLookAndFeel();
        const int fontHeight = 14;
        g.setFont(fontHeight);
        g.setColour(lookAndFeel.findColour(mainTextColourId));
        Rectangle<int> area(width, height);
        String text = _property->name();
        g.drawFittedText(text, area.reduced(4, 2), Justification::left, area.getHeight() / fontHeight);
    }

    virtual bool mightContainSubItems() override
    {
        return dynamic_cast<RC505::Group*>(_property) != nullptr;
    }

    RC505::Property* _property;
};

// ----------------------------------------------------------------------------
// PropertyTreeView
// ----------------------------------------------------------------------------

PropertyTreeView::PropertyTreeView()
    : _labelWidth(150)
    , _valueWidth(150)
{
    setRootItemVisible(false);
    setDefaultOpenness(false);
    setMultiSelectEnabled(false);
    setOpenCloseButtonsVisible(true);
}

PropertyTreeView::~PropertyTreeView()
{
    setRootItem(nullptr);
}

void PropertyTreeView::setGroup(RC505::Group* group)
{
    std::unique_ptr<XmlElement> state = getOpennessState(true);
    auto newRoot = group ? std::make_unique<PropertyTreeViewItem>(group) : nullptr;
    setRootItem(newRoot.get());
    _root = std::move(newRoot);
    if (_root)
    {
        _root->setOpen(true);
    }
    if (state)
    {
        restoreOpennessState(*state, true);
    }
}

void PropertyTreeView::setLabelWidth(int labelWidth)
{
    _labelWidth = labelWidth;
    repaint();
}

void PropertyTreeView::setValueWidth(int valueWidth)
{
    _valueWidth = valueWidth;
    repaint();
}

// ----------------------------------------------------------------------------
// PropertySetTreeViewItem
// ----------------------------------------------------------------------------

class PropertySetTreeViewItem : public TreeViewItem
{
public:
    PropertySetTreeViewItem(RC505::Property* property)
        : _property(property)
    {
    }

    int getItemWidth() const override { return -1; }
    int getItemHeight() const override { return 20; }

    virtual void itemOpennessChanged(bool isNowOpen) override
    {
        if (isNowOpen)
        {
            if (const RC505::Group* group = dynamic_cast<const RC505::Group*>(_property))
            {
                for (auto child : group->children())
                {
                    if (child->visible())
                    {
                        addSubItem(new PropertySetTreeViewItem(child));
                    }
                }
            }
        }
        else
        {
            clearSubItems();
        }
    }

    virtual void paintItem(Graphics& g, int width, int height) override
    {
        auto& lookAndFeel = LookAndFeel::getDefaultLookAndFeel();
        const int fontHeight = 14;
        g.setFont(fontHeight);
        g.setColour(lookAndFeel.findColour(mainTextColourId));
        Rectangle<int> area(height, 0, width - height, height);
        String text = _property->name();
        g.drawFittedText(text, area.reduced(4, 2), Justification::left, area.getHeight() / fontHeight);
        auto state = _property->selectedState();
        lookAndFeel.drawTickBox(g, *getOwnerView(), 2, 2, height - 4, height - 4, state != RC505::Property::Unselected, state != RC505::Property::PartiallySelected, false, false);
    }

    virtual void itemClicked(const MouseEvent& e) override
    {
        ignoreUnused(e);
        switch (_property->selectedState())
        {
            case RC505::Property::Unselected:
                _property->setSelected(true);
                break;
            case RC505::Property::Selected:
                _property->setSelected(false);
                break;
            case RC505::Property::PartiallySelected:
                _property->setSelected(false);
                break;
        }
        treeHasChanged();
    }

    virtual void itemDoubleClicked(const MouseEvent& e) override
    {
        ignoreUnused(e);
    }

    virtual bool mightContainSubItems() override
    {
        return dynamic_cast<RC505::Group*>(_property) != nullptr;
    }

    RC505::Property* _property;
};

// ----------------------------------------------------------------------------
// PropertySetTreeView
// ----------------------------------------------------------------------------

PropertySetTreeView::PropertySetTreeView()
{
    setRootItemVisible(false);
    setDefaultOpenness(false);
    setMultiSelectEnabled(false);
    setOpenCloseButtonsVisible(true);
}

PropertySetTreeView::~PropertySetTreeView()
{
    setRootItem(nullptr);
}

void PropertySetTreeView::setGroup(RC505::Group* group)
{
    _root = std::make_unique<PropertySetTreeViewItem>(group);
    setRootItem(_root.get());
    _root->setOpen(true);
}
