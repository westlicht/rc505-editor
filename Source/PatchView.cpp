#include "../JuceLibraryCode/JuceHeader.h"
#include "PatchView.h"
#include "Utils.h"
#include "AudioEngine.h"

PatchView::PatchView() :
    _patch(nullptr),
    _tabs(TabbedButtonBar::TabsAtTop)
{
    addAndMakeVisible(_namePropertyView);
    addAndMakeVisible(_tempoPropertyView);
    addAndMakeVisible(_tabs);

    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        TrackView *trackView = new TrackView(i);
        trackView->trackLabel().addListener(this);
        trackView->waveformView().addListener(this);
        _trackViews.add(trackView);
        _tracks.addAndMakeVisible(trackView);
    }
    
    _namePropertyView.textEditor().setFont(24.f);
    _tabs.addTab("Tracks", Colours::white, &_tracks, false);
    _tabs.addTab("Settings", Colours::white, &_propertyTreeView, false);

    AudioEngine::instance().addSource(&_looperEngine);
}

PatchView::~PatchView()
{
    AudioEngine::instance().removeSource(&_looperEngine);
}

void PatchView::setPatch(RC505::Patch *patch)
{
    _patch = patch;
    
    _namePropertyView.setProperty(_patch ? _patch->patchName() : nullptr);
    _tempoPropertyView.setProperty(_patch ? _patch->patchSettings()->master->tempo : nullptr);
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->setTrack(_patch ? _patch->tracks()[i] : nullptr);
    }
    _propertyTreeView.setGroup(_patch ? _patch->patchSettings() : nullptr);
    _looperEngine.setPatch(_patch);
}

void PatchView::paint(Graphics &g)
{
    g.fillAll(Colours::white);
}

void PatchView::resized()
{
    _namePropertyView.setBounds(10, 10, 200, 40);
    _tempoPropertyView.setBounds(220, 10, 200, 40);
    _tabs.setBounds(0, 60, getWidth(), getHeight() - 60);

    int trackWidth = _tracks.getWidth() / RC505::Patch::NumTracks;
    int trackHeight = _tracks.getHeight();
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->setBounds(i * trackWidth, 0, trackWidth, trackHeight);
    }
}

void PatchView::trackMoved(int from, int to)
{
    _patch->moveTrack(from, to);
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->setTrack(_patch->tracks()[i]);
    }
}

void PatchView::filesDropped(WaveformView *waveformView, const StringArray &files)
{
    int index = trackIndex(waveformView);
    AudioSampleBuffer buffer;
    for (const auto &file : files) {
        if (index < RC505::Patch::NumTracks && Utils::readAudioFile(file, buffer)) {
            auto track = _patch->tracks()[index];
            track->setAudioBuffer(buffer);
            auto tempoMeasures = Utils::findTempoAndMeasures(buffer);
            track->trackSettings()->recTmp->setValue(int(std::floor(tempoMeasures.first * 10)));
            track->trackSettings()->measLen->setValue(tempoMeasures.second);
            _trackViews[index]->setTrack(track);
            ++index;
        }
    }
}

int PatchView::trackIndex(Component *component) const
{
    while (component != nullptr) {
        if (TrackView *trackView = dynamic_cast<TrackView *>(component)) {
            return trackView->index();
        }
        component = component->getParentComponent();
    }
    jassertfalse;
    return -1;
}
