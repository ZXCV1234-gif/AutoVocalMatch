#include "ReferenceLoader.h"
#include <juce_audio_formats/juce_audio_formats.h>

ReferenceLoader::ReferenceLoader()
{
}

bool ReferenceLoader::load(const juce::File& file)
{
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    std::unique_ptr<juce::AudioFormatReader> reader(
        formatManager.createReaderFor(file)
    );

    if (reader == nullptr)
        return false;

    juce::AudioBuffer<float> buffer(
        (int)reader->numChannels,
        (int)reader->lengthInSamples
    );

    reader->read(&buffer, 0, (int)reader->lengthInSamples, 0, true, true);

    Analyzer analyzer;
    profile = analyzer.analyze(buffer, reader->sampleRate);

    return true;
}

const VocalProfile& ReferenceLoader::getProfile() const
{
    return profile;
}
