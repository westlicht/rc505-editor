#pragma once

#include "JuceHeader.h"

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    AudioSource& source();

    void addSource(AudioSource* source);
    void removeSource(AudioSource* source);

    static AudioEngine& instance();

private:
    static SharedResourcePointer<AudioEngine> _instance;

    MixerAudioSource _mixer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};
