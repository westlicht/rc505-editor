#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class WaveformView : public Component,
                     public FileDragAndDropTarget {
public:
    class Listener {
    public:
        virtual ~Listener() {}
        virtual void waveformViewFilesDropped(WaveformView *waveformView, const StringArray &files) = 0;
        virtual void waveformViewClicked(WaveformView *waveformView) = 0;
    };

    WaveformView();
    ~WaveformView();

    void setAudioBuffer(const AudioSampleBuffer *audioBuffer);

    void setPlaying(bool playing);
    bool isPlaying() const { return _playing; }
    void setPlayPosition(double position);
    double playPosition() const { return _playPosition; }

    void addListener(Listener *listener) { _listeners.add(listener); }
    void removeListener(Listener *listener) { _listeners.remove(listener); }

    // Component
    virtual void paint(Graphics &g) override;
    virtual void resized() override;
    virtual void mouseDown(const MouseEvent &event) override;

    // FileDragAndDropTarget
    virtual bool isInterestedInFileDrag(const StringArray &files) override;
    virtual void filesDropped(const StringArray &files, int x, int y) override;

private:
    AudioThumbnail _audioThumbnail;
    bool _playing;
    double _playPosition;

    static AudioFormatManager _audioFormatManager;
    static AudioThumbnailCache _audioThumbnailCache;

    ListenerList<Listener> _listeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformView)
};
