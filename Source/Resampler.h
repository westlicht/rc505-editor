#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Resampler {
public:
    static bool resample(const AudioSampleBuffer &src, int srcSampleRate, AudioSampleBuffer &dst, int dstSampleRate);
};
