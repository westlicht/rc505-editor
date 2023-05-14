#pragma once

#include "JuceHeader.h"
#include "rubberband/RubberBandStretcher.h"

#include "RC505.h"

class LooperEngine : public AudioSource
{
public:
    class Track : public AudioSource
    {
    public:
        Track(LooperEngine& looperEngine);

        void setTrack(RC505::Track* track);

        void setPlaying(bool playing);
        bool isPlaying() const { return _play; }
        double playPosition() const;

        // AudioSource
        virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        virtual void releaseResources() override;
        virtual void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    private:
        static const int BufferLength = 128;

        LooperEngine& _looperEngine;
        RC505::Track* _track;
        bool _play;

        RubberBand::RubberBandStretcher _stretcher;
        int _inputSampleIndex;

        AudioSampleBuffer _tmpBuffer;
    };

    LooperEngine();
    ~LooperEngine() override;

    void setPatch(RC505::Patch* patch);

    void setPlayingAll(bool playing);
    bool isPlayingAny() const;

    const OwnedArray<Track>& tracks() const { return _tracks; }

    // AudioSource
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    virtual void releaseResources() override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

private:
    double globalTempo() const;
    int tracksPlaying() const;

    double _sampleRate;
    int _sampleIndex;
    RC505::Patch* _patch;

    OwnedArray<Track> _tracks;
    MixerAudioSource _mixer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LooperEngine)
};
