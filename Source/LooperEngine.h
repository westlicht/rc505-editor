#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "rubberband/RubberBandStretcher.h"

#include "RC505.h"

class LooperEngine : public AudioSource {
public:
    class Track : public AudioSource {
    public:
        Track();

        void setTrack(RC505::Track *track);
        void setGlobalTempo(double globalTempo);

        virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        virtual void releaseResources() override;
        virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    private:
        RC505::Track *_track;
        bool _play;
        double _globalTempo;

        RubberBand::RubberBandStretcher _stretcher;
        int _inputSampleIndex;
    };

    LooperEngine();
    ~LooperEngine();

    void setPatch(RC505::Patch *patch);

    // AudioSource
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    virtual void releaseResources() override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

private:
    double _sampleRate;
    RC505::Patch *_patch;

    Track _tracks[RC505::Patch::NumTracks];
    MixerAudioSource _mixer;
};
