#include "LooperEngine.h"
#include "JuceHeader.h"

LooperEngine::Track::Track(LooperEngine& looperEngine)
    : _looperEngine(looperEngine)
    , _track(nullptr)
    , _play(false)
    , _stretcher(44100, 2, RubberBand::RubberBandStretcher::OptionProcessRealTime)
    , _tmpBuffer(2, BufferLength)
{
    _stretcher.setTimeRatio(1.0);
}

void LooperEngine::Track::setTrack(RC505::Track* track)
{
    _track = track;
    _inputSampleIndex = 0;

    setPlaying(_play);
}

void LooperEngine::Track::setPlaying(bool playing)
{
    if (_looperEngine.tracksPlaying() == 0)
    {
        _looperEngine._sampleIndex = 0;
    }
    _stretcher.reset();
    if (_track && _track->audioBuffer().getNumSamples() > 0)
    {
        // ratio > 1 slower / ratio < 1 faster
        double recordTempo = _track->trackSettings()->recTmp->value() / 10.0;
        double ratio = recordTempo / _looperEngine.globalTempo();
        _inputSampleIndex = int(std::floor(_looperEngine._sampleIndex / ratio)) % _track->audioBuffer().getNumSamples();
        _play = playing;
    }
    else
    {
        _inputSampleIndex = 0;
        _play = false;
    }
}

double LooperEngine::Track::playPosition() const
{
    if (!_play)
    {
        return 0.0;
    }
    const AudioSampleBuffer& inputBuffer = _track->audioBuffer();
    double position = double(_inputSampleIndex) / inputBuffer.getNumSamples();
    return _track->trackSettings()->reverse->value() ? 1.0 - position : position;
}

void LooperEngine::Track::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    ignoreUnused(samplesPerBlockExpected, sampleRate);
}

void LooperEngine::Track::releaseResources()
{
}

void LooperEngine::Track::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    if (!_play || !_track || _track->audioBuffer().getNumSamples() == 0)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    double recordTempo = _track->trackSettings()->recTmp->value() / 10.0;
    double ratio = recordTempo / _looperEngine.globalTempo();
    _stretcher.setTimeRatio(ratio);

    int outputSampleIndex = bufferToFill.startSample;
    int outputSamplesLeft = bufferToFill.numSamples;
    const AudioSampleBuffer& inputBuffer = _track->audioBuffer();
    AudioSampleBuffer& outputBuffer = *bufferToFill.buffer;
    while (outputSamplesLeft > 0)
    {
        if (_stretcher.available() > 0)
        {
            int samples = jmin(outputSamplesLeft, _stretcher.available());
            float* const output[2] = {
                outputBuffer.getWritePointer(0, outputSampleIndex),
                outputBuffer.getWritePointer(1, outputSampleIndex)
            };
            _stretcher.retrieve(output, size_t(samples));
            outputSampleIndex += samples;
            outputSamplesLeft -= samples;
        }
        else
        {
            int samples = jmin(BufferLength, inputBuffer.getNumSamples() - _inputSampleIndex);
            if (_track->trackSettings()->reverse->value())
            {
                _tmpBuffer.copyFrom(0, 0, inputBuffer, 0, inputBuffer.getNumSamples() - _inputSampleIndex - samples, samples);
                _tmpBuffer.copyFrom(1, 0, inputBuffer, 1, inputBuffer.getNumSamples() - _inputSampleIndex - samples, samples);
                _tmpBuffer.reverse(0, samples);
            }
            else
            {
                _tmpBuffer.copyFrom(0, 0, inputBuffer, 0, _inputSampleIndex, samples);
                _tmpBuffer.copyFrom(1, 0, inputBuffer, 1, _inputSampleIndex, samples);
            }
            float const* input[2] = {
                _tmpBuffer.getReadPointer(0, 0),
                _tmpBuffer.getReadPointer(1, 0)
            };
            _stretcher.process(input, size_t(samples), false);
            _inputSampleIndex += samples;
            if (_inputSampleIndex >= inputBuffer.getNumSamples())
            {
                _inputSampleIndex = 0;
            }
        }
    }

    float volume = _track->trackSettings()->playLevel->value() / 50.f;
    float pan = (_track->trackSettings()->pan->value() - 50) / 50.f;
    float gain[2] = { volume * jmin(1.f, 1.f - pan), volume * jmin(1.f, 1.f + pan) };
    bufferToFill.buffer->applyGain(0, bufferToFill.startSample, bufferToFill.numSamples, gain[0]);
    bufferToFill.buffer->applyGain(1, bufferToFill.startSample, bufferToFill.numSamples, gain[1]);
}

LooperEngine::LooperEngine()
    : _sampleIndex(0)
    , _patch(nullptr)
{
    for (int i = 0; i < RC505::Patch::NumTracks; ++i)
    {
        _tracks.add(new Track(*this));
        _mixer.addInputSource(_tracks[i], false);
    }
}

LooperEngine::~LooperEngine()
{
    for (int i = 0; i < RC505::Patch::NumTracks; ++i)
    {
        _tracks[i]->setTrack(nullptr);
        _mixer.removeInputSource(_tracks[i]);
    }
}

void LooperEngine::setPatch(RC505::Patch* patch)
{
    _patch = patch;
    if (_patch)
    {
        for (int i = 0; i < RC505::Patch::NumTracks; ++i)
        {
            _tracks[i]->setTrack(_patch->tracks()[i]);
        }
    }
}

void LooperEngine::setPlayingAll(bool playing)
{
    if (_patch)
    {
        for (const auto& track : _tracks)
        {
            track->setPlaying(playing);
        }
    }
}

bool LooperEngine::isPlayingAny() const
{
    for (const auto& track : _tracks)
    {
        if (track->isPlaying())
        {
            return true;
        }
    }
    return false;
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

void LooperEngine::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    if (!_patch)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    _mixer.getNextAudioBlock(bufferToFill);

    _sampleIndex += bufferToFill.numSamples;
}

double LooperEngine::globalTempo() const
{
    return _patch->patchSettings()->master->tempo->value() / 10.0;
}

int LooperEngine::tracksPlaying() const
{
    int count = 0;
    for (auto track : _tracks)
    {
        if (track->isPlaying())
        {
            ++count;
        }
    }
    return count;
}
