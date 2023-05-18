#include "PatchTreeView.h"
#include "CustomLookAndFeel.h"

static const char* PatchItems = "Patch Items";

// ----------------------------------------------------------------------------
// PatchTreeViewItem
// ----------------------------------------------------------------------------

class PatchTreeViewItem : public TreeViewItem
{
public:
    PatchTreeViewItem(RC505::Patch* patch)
        : _patch(patch)
    {
    }

    int getItemWidth() const override { return -1; }
    int getItemHeight() const override { return 20; }

    virtual void paintItem(Graphics& g, int width, int height) override
    {
        auto& lookAndFeel = CustomLookAndFeel::instance();
        const int fontHeight = 12;
        Font font = lookAndFeel.matrixFont();
        font.setHeight(fontHeight);
        g.setFont(font);
        g.setColour(lookAndFeel.findColour(mainTextColourId));
        Rectangle<int> area(width, height);
        String text = String::formatted("%02d: ", _patch->index() + 1) + _patch->name();
        while (text.length() < 21)
            text += " ";
        if (_patch->hasWaveChanged())
            text += "*";
        g.drawFittedText(text, area.reduced(4, 2), Justification::left, area.getHeight() / fontHeight);
    }

    virtual void itemSelectionChanged(bool isNowSelected) override
    {
        ignoreUnused(isNowSelected);
        PatchTreeView* tree = dynamic_cast<PatchTreeView*>(getOwnerView());
        tree->itemSelectionChanged();
    }

    virtual var getDragSourceDescription() override
    {
        return PatchItems;
    }

    virtual bool mightContainSubItems() override
    {
        return false;
    }

    RC505::Patch* _patch;
};

// ----------------------------------------------------------------------------
// RootPatchTreeViewItem
// ----------------------------------------------------------------------------

class RootPatchTreeViewItem : public TreeViewItem
{
public:
    RootPatchTreeViewItem(RC505::Library& library)
        : _library(library)
    {
    }

    virtual void itemOpennessChanged(bool isNowOpen) override
    {
        if (isNowOpen)
        {
            for (auto patch : _library.patches())
            {
                addSubItem(new PatchTreeViewItem(patch));
            }
        }
    }

    virtual bool mightContainSubItems() override
    {
        return true;
    }

    virtual bool isInterestedInDragSource(const DragAndDropTarget::SourceDetails& dragSourceDetails) override
    {
        return dragSourceDetails.description == PatchItems;
    }

    virtual void itemDropped(const DragAndDropTarget::SourceDetails& dragSourceDetails, int insertIndex) override
    {
        if (dragSourceDetails.description != PatchItems)
        {
            return;
        }

        Array<RC505::Patch*> selectedPatches;
        TreeView* tree = dynamic_cast<TreeView*>(dragSourceDetails.sourceComponent.get());
        if (tree != nullptr)
        {
            const int numSelected = tree->getNumSelectedItems();
            for (int i = 0; i < numSelected; ++i)
                if (PatchTreeViewItem* item = dynamic_cast<PatchTreeViewItem*>(tree->getSelectedItem(i)))
                {
                    selectedPatches.add(item->_patch);
                }
        }

        _library.movePatches(selectedPatches, insertIndex);

        getOwnerView()->clearSelectedItems();

        for (int i = 0; i < getNumSubItems(); ++i)
        {
            jassert(i < _library.patches().size());
            if (PatchTreeViewItem* item = dynamic_cast<PatchTreeViewItem*>(getSubItem(i)))
            {
                item->_patch = _library.patches()[i];
                if (selectedPatches.contains(item->_patch))
                {
                    item->setSelected(true, false);
                }
            }
        }

        getOwnerView()->repaint();
    }

private:
    RC505::Library& _library;
};

// ----------------------------------------------------------------------------
// PatchTreeView
// ----------------------------------------------------------------------------

PatchTreeView::PatchTreeView(RC505::Library& library)
    : _library(library)
{
    _library.addListener(this);

    setRootItemVisible(false);
    setDefaultOpenness(true);
    setMultiSelectEnabled(true);
    setOpenCloseButtonsVisible(false);

    afterLibraryLoaded();
}

PatchTreeView::~PatchTreeView()
{
    setRootItem(nullptr);
}

void PatchTreeView::selectPatch(RC505::Patch* patch)
{
    clearSelectedItems();
    for (int i = 0; i < _root->getNumSubItems(); ++i)
    {
        if (PatchTreeViewItem* item = dynamic_cast<PatchTreeViewItem*>(_root->getSubItem(i)))
        {
            if (item->_patch == patch)
            {
                item->setSelected(true, false);
            }
        }
    }
}

RC505::Patch* PatchTreeView::selectedPatch() const
{
    return getNumSelectedItems() > 0 ? static_cast<PatchTreeViewItem*>(getSelectedItem(0))->_patch : nullptr;
}

Array<RC505::Patch*> PatchTreeView::selectedPatches() const
{
    Array<RC505::Patch*> patches;
    for (int i = 0; i < getNumSelectedItems(); ++i)
    {
        patches.add(static_cast<PatchTreeViewItem*>(getSelectedItem(i))->_patch);
    }
    return patches;
}

void PatchTreeView::beforeLibraryLoaded()
{
    setRootItem(nullptr);
}

void PatchTreeView::afterLibraryLoaded()
{
    _root = std::make_unique<RootPatchTreeViewItem>(_library);
    setRootItem(_root.get());
    selectPatch(_library.patches()[0]);
}

void PatchTreeView::propertyValueChanged(RC505::ValueProperty* property)
{
    if (dynamic_cast<RC505::NameProperty*>(property))
    {
        repaint();
    }
}

void PatchTreeView::itemSelectionChanged()
{
    if (getNumSelectedItems() == 1)
    {
        if (PatchTreeViewItem* item = dynamic_cast<PatchTreeViewItem*>(getSelectedItem(0)))
        {
            _listeners.call(&Listener::patchSelected, item->_patch);
        }
    }
}
