#pragma once

#include "JuceHeader.h"

#include "RC505.h"

class RootPatchTreeViewItem;

class PatchTreeView : public TreeView,
                      public RC505::Library::Listener
{
public:
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void patchSelected(RC505::Patch* patch) = 0;
    };

    PatchTreeView(RC505::Library& library);
    ~PatchTreeView() override;

    void selectPatch(RC505::Patch* patch);
    RC505::Patch* selectedPatch() const;
    Array<RC505::Patch*> selectedPatches() const;

    void addListener(Listener* listener) { _listeners.add(listener); }
    void removeListener(Listener* listener) { _listeners.remove(listener); }

    // RC505::Library::Listener
    virtual void beforeLibraryLoaded() override;
    virtual void afterLibraryLoaded() override;
    virtual void propertyValueChanged(RC505::ValueProperty* property) override;

private:
    void itemSelectionChanged();

    RC505::Library& _library;
    std::unique_ptr<RootPatchTreeViewItem> _root;
    ListenerList<Listener> _listeners;

    friend class PatchTreeViewItem;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchTreeView)
};
