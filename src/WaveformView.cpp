#include "WaveformView.h"
#include "CustomLookAndFeel.h"
#include "JuceHeader.h"

AudioFormatManager WaveformView::_audioFormatManager;
AudioThumbnailCache WaveformView::_audioThumbnailCache(8);

WaveformView::WaveformView()
    : _audioThumbnail(256, _audioFormatManager, _audioThumbnailCache)
    , _playing(false)
    , _playPosition(0.0)
{
}

WaveformView::~WaveformView()
{
}

void WaveformView::setAudioBuffer(const AudioSampleBuffer* audioBuffer)
{
    _audioThumbnail.clear();
    if (audioBuffer)
    {
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

void WaveformView::paint(Graphics& g)
{
    const int radius = 10;

    Rectangle<int> area(1, 1, getWidth() - 2, getHeight() - 2);
    g.setColour(findColour(backgroundColourId));
    g.fillRoundedRectangle(area.toFloat(), radius);
    g.setColour(findColour(borderColourId));
    g.drawRoundedRectangle(area.toFloat(), radius, 2.f);

    g.setColour(findColour(waveformColourId).withAlpha(0.5f));
    area = area.withTrimmedLeft(1).withTrimmedRight(1).withTrimmedTop(radius).withTrimmedBottom(radius);
    _audioThumbnail.drawChannels(g, area, 0.0, _audioThumbnail.getTotalLength(), 1.f);
    _audioThumbnail.drawChannels(g, area, 0.0, _audioThumbnail.getTotalLength(), 0.8f);

    if (_playing)
    {
        float x = area.getX() + float(_playPosition) * area.getWidth();
        g.setColour(findColour(cursorColourId));
        g.drawLine(x, area.getY(), x, area.getY() + area.getHeight());
    }
}

void WaveformView::mouseDrag(const MouseEvent& event)
{
    if (event.getDistanceFromDragStart() > 10)
    {
        File file;
        _listeners.call(&Listener::waveformViewFileDragged, this, file);
        if (file.exists())
        {
            performExternalDragDropOfFiles(StringArray({ file.getFullPathName() }), true);
        }
    }
}

void WaveformView::resized()
{
}

bool WaveformView::isInterestedInFileDrag(const StringArray& files)
{
    ignoreUnused(files);
    return true;
}

void WaveformView::filesDropped(const StringArray& files, int x, int y)
{
    ignoreUnused(x, y);
    _listeners.call(&Listener::waveformViewFilesDropped, this, files);
}
