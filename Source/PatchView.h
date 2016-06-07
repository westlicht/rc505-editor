#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "RC505.h"
#include "TrackView.h"
#include "PropertyTreeView.h"
#include "PropertyView.h"
#include "LooperEngine.h"

class PatchView : public Component, 
                  public Timer,
                  public KeyListener,
                  public Button::Listener,
                  public TrackLabel::Listener,
                  public WaveformView::Listener {
public:
    PatchView();
    ~PatchView();
    
    RC505::Patch *patch() const { return _patch; }
    void setPatch(RC505::Patch *patch);
    
    void togglePlay();

    // Component
    virtual void paint(Graphics &g) override;
    virtual void resized() override;

    // Timer
    virtual void timerCallback() override;

    // KeyListener
    virtual bool keyPressed(const KeyPress &key, Component *originatingComponent) override;

    // Button::Listener
    virtual void buttonClicked(Button *button) override;

    // TrackLabel::Listener
    virtual void trackMoved(int from, int to) override;

    // WaveformView::Listener
    virtual void waveformViewFilesDropped(WaveformView *waveformView, const StringArray &files) override;
    virtual void waveformViewClicked(WaveformView *waveformView) override;

private:
    void clearPatch();
    void importLoops();
    void exportLoops();

    void updatePlayState();

    int trackIndex(Component *component) const;

    RC505::Patch *_patch;

    NamePropertyView _namePropertyView;
    IntPropertyView _tempoPropertyView;
    TextButton _playButton;
    TextButton _clearButton;
    TextButton _importButton;
    TextButton _exportButton;

    TabbedComponent _tabs;
    Component _tracks;
    OwnedArray<TrackView> _trackViews;
    PropertyTreeView _propertyTreeView;

    LooperEngine _looperEngine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchView)
};
