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
        trackView->playButton().addListener(this);
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
    
    _looperEngine.setPatch(_patch);
    _looperEngine.setPlayingAll(false);
    updatePlayState();
    _namePropertyView.setProperty(_patch ? _patch->patchName() : nullptr);
    _tempoPropertyView.setProperty(_patch ? _patch->patchSettings()->master->tempo : nullptr);
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->setTrack(_patch ? _patch->tracks()[i] : nullptr);
        _trackViews[i]->waveformView().setPlaying(_looperEngine.tracks()[i]->isPlaying());
    }
    _propertyTreeView.setGroup(_patch ? _patch->patchSettings() : nullptr);    
}

void PatchView::stopPlaying()
{
    _looperEngine.setPlayingAll(false);
    updatePlayState();
}

void PatchView::togglePlaying()
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

void PatchView::visibilityChanged()
{
    if (!isVisible()) {
        _looperEngine.setPlayingAll(false);
    }
}

void PatchView::timerCallback()
{
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->waveformView().setPlaying(_looperEngine.tracks()[i]->isPlaying());
        _trackViews[i]->waveformView().setPlayPosition(_looperEngine.tracks()[i]->playPosition());
    }
}

void PatchView::buttonClicked(Button *button)
{
    if (button == &_playButton) {
        togglePlaying();
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
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        if (button == &_trackViews[i]->playButton()) {
            _looperEngine.tracks()[i]->setPlaying(!_looperEngine.tracks()[i]->isPlaying());
            updatePlayState();
            break;
        }
    }
}

void PatchView::trackMoved(int from, int to)
{
    _patch->moveTrack(from, to);
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->setTrack(_patch->tracks()[i]);
    }
}

void PatchView::waveformViewFilesDropped(WaveformView *waveformView, const StringArray &filenames)
{
    Array<File> files;
    for (const auto &filename : filenames) {
        files.add(File(filename));
    }
    importLoopsToTracks(files, trackIndex(waveformView));
}

void PatchView::waveformViewFileDragged(WaveformView *waveformView, File &file)
{
    auto track = _trackViews[trackIndex(waveformView)]->track();
    if (track->waveState() != RC505::Track::WaveEmpty) {
        file = File::addTrailingSeparator(RC505::Library::tempDirectory()) + String::formatted("track-%d.wav", track->index() + 1);
        track->saveWaveTo(file);
    }
}

void PatchView::clearPatch()
{
    // TODO implement
}

void PatchView::importLoops()
{
    FileChooser fileChooser("Import Loops", File::nonexistent, "*.wav");
    if (fileChooser.browseForMultipleFilesToOpen()) {
        importLoopsToTracks(fileChooser.getResults());
    }
}

void PatchView::exportLoops()
{
    FileChooser fileChooser("Export Loops");
    if (fileChooser.browseForDirectory()) {
        for (const auto &track : _patch->tracks()) {
            File file(File::addTrailingSeparator(fileChooser.getResult().getFullPathName()) + String::formatted("track-%d.wav", track->index() + 1));
            track->saveWaveTo(file);
        }
    }
}

void PatchView::updatePlayState()
{
    _playButton.setPlaying(_looperEngine.isPlayingAny());
    for (int i = 0; i < RC505::Patch::NumTracks; ++i) {
        _trackViews[i]->waveformView().setPlaying(_looperEngine.tracks()[i]->isPlaying());
        _trackViews[i]->playButton().setPlaying(_looperEngine.tracks()[i]->isPlaying());
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

void PatchView::importLoopsToTracks(const Array<File> &files, int trackIndex)
{
    bool empty = true;
    for (const auto &track : _patch->tracks()) {
        if (track->waveState() != RC505::Track::WaveEmpty) {
            empty = false;
        }
    }

    AudioSampleBuffer buffer;
    for (const auto &file : files) {
        if (trackIndex < RC505::Patch::NumTracks && Utils::readAudioFile(file, buffer)) {
            auto track = _patch->tracks()[trackIndex];
            track->setAudioBuffer(buffer);
            auto tempoMeasures = Utils::findTempoAndMeasures(buffer);
            track->trackSettings()->recTmp->setValue(int(std::floor(tempoMeasures.first * 10)));
            track->trackSettings()->measLen->setValue(tempoMeasures.second);
            if (empty) {
                _patch->patchSettings()->master->tempo->setValue(int(std::floor(tempoMeasures.first * 10)));
                empty = false;
            }
            _trackViews[trackIndex]->setTrack(track);
            ++trackIndex;
        }
    }
}
