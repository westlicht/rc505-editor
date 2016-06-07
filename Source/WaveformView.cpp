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

    int cx = getWidth() / 2;
    int cy = getHeight() / 2;
    if (_playing) {
        // draw stop icon
        g.setColour(Colours::red);
        g.fillRect(cx - 5, cy - 5, 10, 10);
    } else {
        // draw play icon
        Path path;
        path.startNewSubPath(cx - 5, cy - 5);
        path.lineTo(cx + 5, cy);
        path.lineTo(cx - 5, cy + 5);
        path.closeSubPath();
        g.setColour(Colours::green);
        g.fillPath(path);
    }
}

void WaveformView::resized()
{
}

void WaveformView::mouseDown(const MouseEvent &event)
{
    _listeners.call(&Listener::waveformViewClicked, this);
}

bool WaveformView::isInterestedInFileDrag(const StringArray &files)
{
    return true;
}

void WaveformView::filesDropped(const StringArray &files, int x, int y)
{
    _listeners.call(&Listener::waveformViewFilesDropped, this, files);
}
