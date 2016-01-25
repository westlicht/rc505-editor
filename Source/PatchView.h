#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "RC505.h"
#include "TrackView.h"
#include "PropertyTreeView.h"
#include "PropertyView.h"
#include "LooperEngine.h"

class PatchView : public Component, TrackLabel::Listener, WaveformView::Listener {
public:
    PatchView();
    ~PatchView();
    
    RC505::Patch *patch() const { return _patch; }
    void setPatch(RC505::Patch *patch);

    void paint(Graphics &g) override;
    void resized() override;

    // TrackLabel::Listener
    void trackMoved(int from, int to) override;

    // WaveformView::Listener
    void filesDropped(WaveformView *waveformView, const StringArray &files) override;

private:
    int trackIndex(Component *component) const;

    RC505::Patch *_patch;

    NamePropertyView _namePropertyView;
    IntPropertyView _tempoPropertyView;

    TabbedComponent _tabs;
    Component _tracks;
    OwnedArray<TrackView> _trackViews;
    PropertyTreeView _propertyTreeView;

    LooperEngine _looperEngine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatchView)
};
