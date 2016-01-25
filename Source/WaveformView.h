#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class WaveformView : public Component, public FileDragAndDropTarget {
public:
    class Listener {
    public:
        virtual ~Listener() {}
        virtual void filesDropped(WaveformView *waveformView, const StringArray &files) = 0;
    };

    WaveformView();
    ~WaveformView();

    void setAudioBuffer(const AudioSampleBuffer *audioBuffer);

    void addListener(Listener *listener) { _listeners.add(listener); }
    void removeListener(Listener *listener) { _listeners.remove(listener); }

    void paint(Graphics &g);
    void resized();

    // FileDragAndDropTarget
    virtual bool isInterestedInFileDrag(const StringArray &files) override;
    virtual void filesDropped(const StringArray &files, int x, int y) override;

private:
    AudioThumbnail _audioThumbnail;

    static AudioFormatManager _audioFormatManager;
    static AudioThumbnailCache _audioThumbnailCache;

    ListenerList<Listener> _listeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformView)
};
