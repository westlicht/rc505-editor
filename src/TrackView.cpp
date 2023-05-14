#include "TrackView.h"
#include "CustomLookAndFeel.h"
#include "JuceHeader.h"

void TrackViewGroup::paint(Graphics& g)
{
    g.fillAll(findColour(mainBackgroundColourId));
    g.setColour(findColour(mainBorderColourId));
    int trackWidth = getWidth() / RC505::Patch::NumTracks;
    for (int i = 1; i < RC505::Patch::NumTracks; ++i)
    {
        int x = i * trackWidth;
        g.drawLine(x + 0.5f, 0, x + 0.5f, getHeight());
    }
}

TrackView::TrackView(int index)
    : _index(index)
    , _track(nullptr)
    , _trackLabel(index)
{
    _propertyTreeView.setOpenCloseButtonsVisible(false);

    addAndMakeVisible(_trackLabel);
    addAndMakeVisible(_waveformView);
    addAndMakeVisible(_playButton);
    addAndMakeVisible(_propertyTreeView);

    _trackLabel.setTooltip("Track number. Drag and drop over a different track number to exchange tracks.");
    _waveformView.setTooltip("Waveform view. Drag files from desktop to import a loop, drag waveform to desktop to export a loop.");
    _playButton.setTooltip("Start/stop playing a single track.");
    _propertyTreeView.setTooltip("Track settings.");
    _propertyTreeView.setLabelWidth(100);
}

TrackView::~TrackView()
{
    removeAllChildren();
}

void TrackView::setTrack(RC505::Track* track)
{
    _track = track;
    _propertyTreeView.setGroup(_track ? _track->trackSettings() : nullptr);
    _waveformView.setAudioBuffer(_track ? &_track->audioBuffer() : nullptr);
}

void TrackView::paint(Graphics& g)
{
    ignoreUnused(g);
}

void TrackView::resized()
{
    _trackLabel.setBounds(10, 10, getWidth() - 20, 30);
    _waveformView.setBounds(10, 50, getWidth() - 20, 100);
    _playButton.setBounds(10, 160, getWidth() - 20, 30);
    _propertyTreeView.setBounds(10, 220, getWidth() - 20, getHeight() - 220);
}
