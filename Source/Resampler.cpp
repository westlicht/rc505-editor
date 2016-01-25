#include "Resampler.h"

#include "libsamplerate/samplerate.h"

bool Resampler::resample(const AudioSampleBuffer &src, int srcSampleRate, AudioSampleBuffer &dst, int dstSampleRate)
{
    int channels = src.getNumChannels();
    int64 srcSamples = src.getNumSamples();
    int64 dstSamples = (srcSamples * dstSampleRate) / srcSampleRate;
    double ratio = double(dstSampleRate) / srcSampleRate;
    dst.setSize(channels, dstSamples);
    for (int channel = 0; channel < channels; ++channel) {
        SRC_DATA srcData;
        srcData.data_in = const_cast<float *>(src.getReadPointer(channel));
        srcData.data_out = dst.getWritePointer(channel);
        srcData.input_frames = srcSamples;
        srcData.output_frames = dstSamples;
        srcData.src_ratio = ratio;
        int ret = src_simple(&srcData, SRC_SINC_MEDIUM_QUALITY, 1);
        if (ret != 0) {
            Logger::outputDebugString(String::formatted("Resampler error: %s", src_strerror(ret)));
            return false;
        }
        if (srcData.input_frames_used != srcData.input_frames) {
            Logger::outputDebugString("Resampler: not all input samples used!");
        }
        if (srcData.output_frames_gen != srcData.output_frames) {
            Logger::outputDebugString("Resampler: not all output samples used!");
        }
    }
    return true;
}
