#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformView.h"

AudioFormatManager WaveformView::_audioFormatManager;
AudioThumbnailCache WaveformView::_audioThumbnailCache(8);

WaveformView::WaveformView() :
    _audioThumbnail(256, _audioFormatManager, _audioThumbnailCache),
    _playing(false),
    _playPosition(0.0)
{
}

WaveformView::~WaveformView()
{
}

void WaveformView::setAudioBuffer(const AudioSampleBuffer *audioBuffer)
{
    _audioThumbnail.clear();
    if (audioBuffer) {
        _audioThumbnail.reset(2, 44100, audioBuffer->getNumSamples());
        _audioThumbnail.addBlock(0, *audioBuffer, 0, audioBuffer->getNumSamples());
    }
    repaint();
}

void WaveformView::setPlaying(bool playing)
{
    _playing = playing;
    repaint();
}

void WaveformView::setPlayPosition(double position)
{
    _playPosition = position;
    repaint();
}

void WaveformView::paint(Graphics &g)
{
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    Rectangle<int> area(0, 0, getWidth(), getHeight());
    _audioThumbnail.drawChannels(g, area, 0.0, _audioThumbnail.getTotalLength(), 1.f);

    if (_playing) {
        float x = _playPosition * getWidth();
        g.setColour(Colours::yellow);
        g.drawLine(x, 0, x, getHeight());
    }
}

void WaveformView::mouseDrag(const MouseEvent &event)
{
    if (event.getDistanceFromDragStart() > 10) {
        File file;
        _listeners.call(&Listener::waveformViewFileDragged, this, file);
        if (file.exists()) {
            performExternalDragDropOfFiles(StringArray({file.getFullPathName()}), true);
        }
    }
}

void WaveformView::resized()
{
}

bool WaveformView::isInterestedInFileDrag(const StringArray &files)
{
    return true;
}

void WaveformView::filesDropped(const StringArray &files, int x, int y)
{
    _listeners.call(&Listener::waveformViewFilesDropped, this, files);
}
