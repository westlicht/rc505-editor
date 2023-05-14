#include "AudioEngine.h"
#include "JuceHeader.h"

SharedResourcePointer<AudioEngine> AudioEngine::_instance;

AudioEngine::AudioEngine()
{
}

AudioEngine::~AudioEngine()
{
}

AudioSource& AudioEngine::source()
{
    return _mixer;
}

void AudioEngine::addSource(AudioSource* source)
{
    _mixer.addInputSource(source, false);
}

void AudioEngine::removeSource(AudioSource* source)
{
    _mixer.removeInputSource(source);
}

AudioEngine& AudioEngine::instance()
{
    return _instance.get();
}
