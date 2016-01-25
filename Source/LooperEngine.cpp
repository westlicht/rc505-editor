#include "../JuceLibraryCode/JuceHeader.h"
#include "LooperEngine.h"

LooperEngine::Track::Track() :
    _track(nullptr),
    _play(false),
    _stretcher(44100, 2, RubberBand::RubberBandStretcher::OptionProcessRealTime)
{
    _stretcher.setTimeRatio(1.0);
}

void LooperEngine::Track::setTrack(RC505::Track *track)
{
    _track = track;
    _inputSampleIndex = 0;
}

void LooperEngine::Track::setGlobalTempo(double globalTempo)
{
    _globalTempo = globalTempo;
}

void LooperEngine::Track::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{

}

void LooperEngine::Track::releaseResources()
{

}

void LooperEngine::Track::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    if (!_play || !_track || _track->audioBuffer().getNumSamples() == 0) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    double recordTempo = _track->trackSettings()->recTmp->value() / 10.0;
    double ratio = recordTempo / _globalTempo;
    _stretcher.setTimeRatio(ratio);

    int outputSampleIndex = bufferToFill.startSample;
    int outputSamplesLeft = bufferToFill.numSamples;
    const AudioSampleBuffer &inputBuffer = _track->audioBuffer();
    AudioSampleBuffer &outputBuffer = *bufferToFill.buffer;
    while (outputSamplesLeft > 0) {
        if (_stretcher.available() > 0) {
            int samples = jmin(outputSamplesLeft, _stretcher.available());
            float * const output[2] = {
                outputBuffer.getWritePointer(0, outputSampleIndex),
                outputBuffer.getWritePointer(1, outputSampleIndex)
            };
            _stretcher.retrieve(output, samples);
            outputSampleIndex += samples;
            outputSamplesLeft -= samples;
        } else {
            int samples = jmin(128, inputBuffer.getNumSamples() - _inputSampleIndex);
            float const *input[2] = {
                inputBuffer.getReadPointer(0, _inputSampleIndex),
                inputBuffer.getReadPointer(1, _inputSampleIndex)
            };
            _stretcher.process(input, samples, false);
            _inputSampleIndex += samples;
            if (_inputSampleIndex >= inputBuffer.getNumSamples()) {
                _inputSampleIndex = 0;
            }
        }
    }

    float volume = _track->trackSettings()->playLevel->value() / 50.f;
    float pan = (_track->trackSettings()->pan->value() - 50) / 50.f;
    float gain[2] = { volume - jmin(0.f, pan), volume + jmax(0.f, pan) };
    bufferToFill.buffer->applyGain(0, bufferToFill.startSample, bufferToFill.numSamples, gain[0]);
    bufferToFill.buffer->applyGain(1, bufferToFill.startSample, bufferToFill.numSamples, gain[1]);
}


LooperEngine::LooperEngine() :
    _patch(nullptr)
{
    for (auto &track : _tracks) {
        _mixer.addInputSource(&track, false);
    }
}

LooperEngine::~LooperEngine()
{
    for (auto &track : _tracks) {
        track.setTrack(nullptr);
        _mixer.removeInputSource(&track);
    }
}

void LooperEngine::setPatch(RC505::Patch *patch)
{
    _patch = patch;
    if (_patch) {
        for (int i = 0; i < RC505::Patch::NumTracks; ++i)  {
            _tracks[i].setTrack(_patch->tracks()[i]);
        }
    }
}

void LooperEngine::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    _mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    _sampleRate = sampleRate;
}

void LooperEngine::releaseResources()
{
    _mixer.releaseResources();
}

void LooperEngine::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    if (!_patch) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    double globalTempo = _patch->patchSettings()->master->tempo->value() / 10.0;
    for (auto &track : _tracks) {
        track.setGlobalTempo(globalTempo);
    }

    _mixer.getNextAudioBlock(bufferToFill);
}
