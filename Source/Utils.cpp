#include "../JuceLibraryCode/JuceHeader.h"
#include "Utils.h"
#include "Resampler.h"

namespace Utils {

bool isValidAudioFile(const File &file)
{
    AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats();
    ScopedPointer<AudioFormatReader> reader = audioFormatManager.createReaderFor(file);
    return reader != nullptr;
}

bool readAudioFile(const File &file, AudioSampleBuffer &buffer, int channels, int sampleRate)
{
    AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats();
    ScopedPointer<AudioFormatReader> reader = audioFormatManager.createReaderFor(file);
    if (!reader) {
        return false;
    }

    buffer.setSize(channels, reader->lengthInSamples);
    reader->read(&buffer, 0, reader->lengthInSamples, 0, true, true);

    // Convert sample rate if necessary
    if (int(reader->sampleRate) != sampleRate) {
        AudioSampleBuffer temp;
        Resampler::resample(buffer, reader->sampleRate, temp, sampleRate);
        buffer = temp;
    }
    return true;
}

bool writeAudioFile(const File &file, const AudioSampleBuffer &buffer, int channels, int sampleRate, int bits)
{
    AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats();
    AudioFormat *format = audioFormatManager.findFormatForFileExtension(file.getFileExtension());
    if (!format) {
        return false;
    }

    ScopedPointer<AudioFormatWriter> writer = format->createWriterFor(new FileOutputStream(file), sampleRate, channels, bits, StringPairArray(), 0);
    if (!writer) {
        return false;
    }
    return writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}

std::pair<double, int> findTempoAndMeasures(const AudioSampleBuffer &buffer, int sampleRate)
{
    const double guideTempo = 120.0;
    std::pair<double, int> result = { 0, 0 };
    int samples = buffer.getNumSamples();
    double duration = double(samples) / sampleRate;
    for (int measure = 1; measure <= 64; measure *= 2) {
        int beats = 4 * measure;
        double beatDuration = duration / beats;
        double tempo = 60.0 / beatDuration;
        if (result.second == 0 || std::abs(guideTempo - tempo) < std::abs(guideTempo - result.first)) {
            result = { tempo, measure };
        }
    }
    return result;
}

} // namespace Utils
