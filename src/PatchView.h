#pragma once

#include "JuceHeader.h"

#include "LooperEngine.h"
#include "PlayButton.h"
#include "PropertyTreeView.h"
#include "PropertyView.h"
#include "RC505.h"
#include "TrackView.h"

class PatchView : public Component,
                  public Timer,
                  public Button::Listener,
                  public TrackLabel::Listener,
                  public WaveformView::Listener
{
public:
    PatchView();
    ~PatchView() override;

    RC505::Patch* patch() const { return _patch; }
    void setPatch(RC505::Patch* patch);

    void stopPlaying();
    void togglePlaying();

    // Component
    virtual void paint(Graphics& g) override;
    virtual void resized() override;
    virtual void visibilityChanged() override;

    // Timer
    virtual void timerCallback() override;

    // Button::Listener
    virtual void buttonClicked(Button* button) override;

    // TrackLabel::Listener
    virtual void trackMoved(int from, int to) override;

    // WaveformView::Listener
    virtual void waveformViewFilesDropped(WaveformView* waveformView, const StringArray& files) override;
    virtual void waveformViewFileDragged(WaveformView* waveformView, File& file) override;

private:
    void clearPatch();
    void importLoops();
    void exportLoops();

    void updatePlayState();

    int trackIndex(Component* component) const;

    void importLoopsToTracks(const Array<File>& files, int trackIndex = 0);

    RC505::Patch* _patch;

    NamePropertyView _namePropertyView;
    IntPropertyView _tempoPropertyView;
    PlayButton _playButton;
    TextButton _clearButton;
    TextButton _importButton;
    TextButton _exportButton;

    TabbedComponent _tabs;
    TrackViewGroup _trackViewGroup;
    OwnedArray<TrackView> _trackViews;
    PropertyTreeView _propertyTreeView;

    LooperEngine _looperEngine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchView)
};
