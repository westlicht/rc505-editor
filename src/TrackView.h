#pragma once

#include "JuceHeader.h"

#include "PlayButton.h"
#include "PropertyTreeView.h"
#include "RC505.h"
#include "TrackLabel.h"
#include "WaveformView.h"

class TrackViewGroup : public Component
{
public:
    virtual void paint(Graphics& g) override;
};

class TrackView : public Component
{
public:
    TrackView(int index);
    ~TrackView() override;

    int index() const { return _index; }

    RC505::Track* track() const { return _track; }
    void setTrack(RC505::Track* track);

    TrackLabel& trackLabel() { return _trackLabel; }
    WaveformView& waveformView() { return _waveformView; }
    PlayButton& playButton() { return _playButton; }

    // Component
    virtual void paint(Graphics& g) override;
    virtual void resized() override;

private:
    int _index;
    RC505::Track* _track;
    TrackLabel _trackLabel;
    WaveformView _waveformView;
    PlayButton _playButton;
    PropertyTreeView _propertyTreeView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackView)
};
