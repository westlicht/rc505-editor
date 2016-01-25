#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace Utils {

bool isValidAudioFile(const File &file);
bool readAudioFile(const File &file, AudioSampleBuffer &buffer, int channels = 2, int sampleRate = 44100);
bool writeAudioFile(const File &file, const AudioSampleBuffer &buffer, int channels = 2, int sampleRate = 44100, int bits = 16);

std::pair<double, int> findTempoAndMeasures(const AudioSampleBuffer &buffer, int sampleRate = 44100);

} // namespace Utils
