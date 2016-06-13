#include "../JuceLibraryCode/JuceHeader.h"
#include "TrackView.h"

TrackView::TrackView(int index) :
    _index(index),
    _track(nullptr),
    _trackLabel(index)
{
    _propertyTreeView.setOpenCloseButtonsVisible(false);

    addAndMakeVisible(_trackLabel);
    addAndMakeVisible(_waveformView);
    addAndMakeVisible(_playButton);
    addAndMakeVisible(_propertyTreeView);
}

TrackView::~TrackView()
{
    removeAllChildren();
}

void TrackView::setTrack(RC505::Track *track)
{
    _track = track;
    _propertyTreeView.setGroup(_track ? _track->trackSettings() : nullptr);
    _waveformView.setAudioBuffer(_track ? &_track->audioBuffer() : nullptr);
}

void TrackView::paint(Graphics &g)
{
    g.fillAll(Colours::white);
}

void TrackView::resized()
{
    _trackLabel.setBounds(5, 5, getWidth() - 10, 30);
    _waveformView.setBounds(5, 40, getWidth() - 10, 100);
    _playButton.setBounds(5, 145, getWidth() - 10, 25);
    _propertyTreeView.setBounds(5, 180, getWidth() - 10, getHeight() - 180);
}
