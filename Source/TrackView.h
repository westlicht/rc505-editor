#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "RC505.h"
#include "TrackLabel.h"
#include "WaveformView.h"
#include "PropertyTreeView.h"

class TrackView : public Component {
public:
    TrackView(int index);
    ~TrackView();

    int index() const { return _index; }

    RC505::Track *track() const { return _track; }
    void setTrack(RC505::Track *track);

    const TrackLabel &trackLabel() const { return _trackLabel; }
          TrackLabel &trackLabel()       { return _trackLabel; }

    const WaveformView &waveformView() const { return _waveformView; }
          WaveformView &waveformView()       { return _waveformView; }

    void paint(Graphics &g);
    void resized();

private:
    int _index;
    RC505::Track *_track;
    TrackLabel _trackLabel;
    WaveformView _waveformView;
    PropertyTreeView _propertyTreeView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackView)
};
