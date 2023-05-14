#include "Utils.h"
#include "JuceHeader.h"

#include "libsamplerate/samplerate.h"

bool Utils::resample(const AudioSampleBuffer& src, int srcSampleRate, AudioSampleBuffer& dst, int dstSampleRate)
{
    int channels = src.getNumChannels();
    int srcSamples = src.getNumSamples();
    int dstSamples = (srcSamples * dstSampleRate) / srcSampleRate;
    double ratio = double(dstSampleRate) / srcSampleRate;
    dst.setSize(channels, dstSamples);
    for (int channel = 0; channel < channels; ++channel)
    {
        SRC_DATA srcData;
        srcData.data_in = const_cast<float*>(src.getReadPointer(channel));
        srcData.data_out = dst.getWritePointer(channel);
        srcData.input_frames = srcSamples;
        srcData.output_frames = dstSamples;
        srcData.src_ratio = ratio;
        int ret = src_simple(&srcData, SRC_SINC_MEDIUM_QUALITY, 1);
        if (ret != 0)
        {
            Logger::outputDebugString(String::formatted("Resampler error: %s", src_strerror(ret)));
            return false;
        }
        if (srcData.input_frames_used != srcData.input_frames)
        {
            Logger::outputDebugString("Resampler: not all input samples used!");
        }
        if (srcData.output_frames_gen != srcData.output_frames)
        {
            Logger::outputDebugString("Resampler: not all output samples used!");
        }
    }
    return true;
}

bool Utils::isValidAudioFile(const File& file)
{
    AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats();
    std::unique_ptr<AudioFormatReader> reader(audioFormatManager.createReaderFor(file));
    return reader != nullptr;
}

bool Utils::readAudioFile(const File& file, AudioSampleBuffer& buffer, int channels, int sampleRate)
{
    AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats();
    std::unique_ptr<AudioFormatReader> reader(audioFormatManager.createReaderFor(file));
    if (!reader)
    {
        return false;
    }

    buffer.setSize(channels, int(reader->lengthInSamples));
    reader->read(&buffer, 0, int(reader->lengthInSamples), 0, true, true);

    // Convert sample rate if necessary
    if (int(reader->sampleRate) != sampleRate)
    {
        AudioSampleBuffer temp;
        resample(buffer, int(reader->sampleRate), temp, sampleRate);
        buffer = temp;
    }
    return true;
}

bool Utils::writeAudioFile(const File& file, const AudioSampleBuffer& buffer, int channels, int sampleRate, int bits)
{
    AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats();
    AudioFormat* format = audioFormatManager.findFormatForFileExtension(file.getFileExtension());
    if (!format)
    {
        return false;
    }

    std::unique_ptr<AudioFormatWriter> writer(format->createWriterFor(new FileOutputStream(file), sampleRate, (unsigned int)channels, bits, StringPairArray(), 0));
    if (!writer)
    {
        return false;
    }
    return writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}

std::pair<double, int> Utils::findTempoAndMeasures(const AudioSampleBuffer& buffer, int sampleRate)
{
    const double guideTempo = 120.0;
    std::pair<double, int> result = { 0, 0 };
    int samples = buffer.getNumSamples();
    double duration = double(samples) / sampleRate;
    for (int measure = 1; measure <= 64; measure *= 2)
    {
        int beats = 4 * measure;
        double beatDuration = duration / beats;
        double tempo = 60.0 / beatDuration;
        if (result.second == 0 || std::abs(guideTempo - tempo) < std::abs(guideTempo - result.first))
        {
            result = { tempo, measure };
        }
    }
    return result;
}
