#pragma once

#include "JuceHeader.h"

class Utils
{
public:
    static bool resample(const AudioSampleBuffer& src, int srcSampleRate, AudioSampleBuffer& dst, int dstSampleRate);

    // Returns true if the given file is a valid audio file.
    static bool isValidAudioFile(const File& file);
    // Reads an audio file into an AudioSampleBuffer and resamples to match the requested sample rate.
    static bool readAudioFile(const File& file, AudioSampleBuffer& buffer, int channels = 2, int sampleRate = 44100);
    // Writes an AudioSampleBuffer to an audio file with the given settings.
    static bool writeAudioFile(const File& file, const AudioSampleBuffer& buffer, int channels = 2, int sampleRate = 44100, int bits = 16);

    // Finds a reasonable tempo and measure count for a given AudioSampleBuffer.
    static std::pair<double, int> findTempoAndMeasures(const AudioSampleBuffer& buffer, int sampleRate = 44100);
};
