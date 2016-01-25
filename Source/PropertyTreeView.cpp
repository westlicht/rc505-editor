#include "PropertyTreeView.h"
#include "PropertyView.h"

// ----------------------------------------------------------------------------
// PropertyTreeViewComponent
// ----------------------------------------------------------------------------

template<typename TProperty, typename TPropertyView>
class PropertyTreeViewComponent : public Component {
public:
    PropertyTreeViewComponent(TProperty *property) :
        _property(property),
        _propertyView(property)
    {
        addAndMakeVisible(_propertyView);
    }

    virtual void resized() override
    {
        const int marginLeft = 150;
        const int maxWidth = 250;
        _propertyView.setBounds(marginLeft, 0, jmin(maxWidth, getWidth() - marginLeft), getHeight());
    }

private:
    TProperty *_property;
    TPropertyView _propertyView;
};

// ----------------------------------------------------------------------------
// PropertyTreeViewItem
// ----------------------------------------------------------------------------

class PropertyTreeViewItem : public TreeViewItem {
public:
    PropertyTreeViewItem(RC505::Property *property) :
        _property(property)
    {
    }

    int getItemWidth() const override { return -1; }
    int getItemHeight() const override { return 20; }

    virtual void itemOpennessChanged(bool isNowOpen) override
    {
        if (isNowOpen) {
            if (const RC505::Group *group = dynamic_cast<const RC505::Group *>(_property)) {
                for (auto child : group->children()) {
                    if (!child->hidden()) {
                        addSubItem(new PropertyTreeViewItem(child));
                    }
                }
            }
        } else {
            clearSubItems();
        }
    }

    virtual Component *createItemComponent() override
    {
        if (RC505::BoolProperty *boolProperty = dynamic_cast<RC505::BoolProperty *>(_property)) {
            return new PropertyTreeViewComponent<RC505::BoolProperty, BoolPropertyView>(boolProperty);
        } else if (RC505::IntProperty *intProperty = dynamic_cast<RC505::IntProperty *>(_property)) {
            return new PropertyTreeViewComponent<RC505::IntProperty, IntPropertyView>(intProperty);
        } else  if (RC505::EnumProperty *enumProperty = dynamic_cast<RC505::EnumProperty *>(_property)) {
            return new PropertyTreeViewComponent<RC505::EnumProperty, EnumPropertyView>(enumProperty);
        } else  if (RC505::NameProperty *nameProperty = dynamic_cast<RC505::NameProperty *>(_property)) {
            return new PropertyTreeViewComponent<RC505::NameProperty, NamePropertyView>(nameProperty);
        } else  if (RC505::ValueProperty *valueProperty = dynamic_cast<RC505::ValueProperty *>(_property)) {
            return new PropertyTreeViewComponent<RC505::ValueProperty, ValuePropertyView>(valueProperty);
        }
        return nullptr;
    }

    virtual void paintItem(Graphics &g, int width, int height) override
    {
        const int fontHeight = 14;
        g.setFont(fontHeight);
        g.setColour(Colours::black);
        Rectangle<int> area(width, height);
        String text = _property->name();
        g.drawFittedText(text, area.reduced(4, 2), Justification::left, area.getHeight() / fontHeight);
    }

    virtual bool mightContainSubItems() override
    {
        return dynamic_cast<RC505::Group *>(_property) != nullptr;
    }

    RC505::Property *_property;
};

// ----------------------------------------------------------------------------
// PropertyTreeView
// ----------------------------------------------------------------------------

PropertyTreeView::PropertyTreeView()
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

void PropertyTreeView::setGroup(RC505::Group *group)
{
    _root = new PropertyTreeViewItem(group);
    setRootItem(_root);
    _root->setOpen(true);
}
