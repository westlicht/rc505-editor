#include "../JuceLibraryCode/JuceHeader.h"
#include "PatchView.h"
#include "Utils.h"
#include "AudioEngine.h"

PatchView::PatchView() :
    _patch(nullptr),
    _tabs(TabbedButtonBar::TabsAtTop)
{
    addKeyListener(this);

    addAndMakeVisible(_namePropertyView);
    addAndMakeVisible(_tempoPropertyView);
    addAndMakeVisible(_playButton);
    addAndMakeVisible(_clearButton);
    addAndMakeVisible(_importButton);
    addAndMakeVisible(_exportButton);
    addAndMakeVisible(_tabs);
    
    _playButton.setButtonText("Play");
    _playButton.addListener(this);
    _clearButton.setButtonText("Clear Patch");
    _clearButton.addListener(this);
    _importButton.setButtonText("Import Loops");
    _importButton.addListener(this);
    _exportButton.setButtonText("Export Loops");
    _exportButton.addListener(this);

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

    startTimer(50);
}

PatchView::~PatchView()
{
    stopTimer();

    AudioEngine::instance().removeSource(&_looperEngine);

    removeAllChildren();
}

void PatchView::setPatch(RC505::Patch *patch)
{
    _patch = patch;
    
    _looperEngine.setPlayingAll(false);
    updatePlayState();
    _looperEngine.setPatch(_patch);
    _namePropertyView.setProperty(_patch ? _patch->patchName() : nullptr);
    _tempoPropertyView.setProperty(_patch ? _patch->patchSettings()->master->tempo : nullptr);
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->setTrack(_patch ? _patch->tracks()[i] : nullptr);
        _trackViews[i]->waveformView().setPlaying(_looperEngine.tracks()[i]->isPlaying());
    }
    _propertyTreeView.setGroup(_patch ? _patch->patchSettings() : nullptr);    
}

void PatchView::togglePlay()
{
    _looperEngine.setPlayingAll(!_looperEngine.isPlayingAny());
    updatePlayState();
}

void PatchView::paint(Graphics &g)
{
    g.fillAll(Colours::white);
}

void PatchView::resized()
{
    _namePropertyView.setBounds(10, 10, 200, 40);
    _tempoPropertyView.setBounds(220, 10, 200, 40);
    _playButton.setBounds(500, 15, 110, 30);
    _clearButton.setBounds(620, 15, 110, 30);
    _importButton.setBounds(740, 15, 110, 30);
    _exportButton.setBounds(860, 15, 110, 30);
    _tabs.setBounds(0, 60, getWidth(), getHeight() - 60);

    int trackWidth = _tracks.getWidth() / RC505::Patch::NumTracks;
    int trackHeight = _tracks.getHeight();
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->setBounds(i * trackWidth, 0, trackWidth, trackHeight);
    }
}

void PatchView::timerCallback()
{
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->waveformView().setPlaying(_looperEngine.tracks()[i]->isPlaying());
        _trackViews[i]->waveformView().setPlayPosition(_looperEngine.tracks()[i]->playPosition());
    }
}

bool PatchView::keyPressed(const KeyPress &key, Component *originatingComponent)
{
    return false;
}

void PatchView::buttonClicked(Button *button)
{
    if (button == &_playButton) {
        togglePlay();
    }
    if (button == &_clearButton) {
        clearPatch();
    }
    if (button == &_importButton) {
        importLoops();
    }
    if (button == &_exportButton) {
        exportLoops();
    }
}

void PatchView::trackMoved(int from, int to)
{
    _patch->moveTrack(from, to);
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->setTrack(_patch->tracks()[i]);
    }
}

void PatchView::waveformViewFilesDropped(WaveformView *waveformView, const StringArray &files)
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

void PatchView::waveformViewClicked(WaveformView *waveformView)
{
    int index = trackIndex(waveformView);
    _looperEngine.tracks()[index]->setPlaying(!_looperEngine.tracks()[index]->isPlaying());
    updatePlayState();
}

void PatchView::clearPatch()
{
    // TODO
}

void PatchView::importLoops()
{
    // TODO
}

void PatchView::exportLoops()
{
    // TODO
}

void PatchView::updatePlayState()
{
    _playButton.setButtonText(_looperEngine.isPlayingAny() ? "Stop" : "Play");
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->waveformView().setPlaying(_looperEngine.tracks()[i]->isPlaying());
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
