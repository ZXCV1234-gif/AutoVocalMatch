#include "Analyzer.h"
#include <juce_dsp/juce_dsp.h>

Analyzer::Analyzer()
    : fft(12) // 4096 点 FFT
{
    fftWindow.setSize(1, fft.getSize());
    juce::dsp::Windowing::fillWindowingTables(
        fftWindow.getWritePointer(0),
        fft.getSize(),
        juce::dsp::Windowing::WindowType::hann,
        true
    );
}

VocalProfile Analyzer::analyze(const juce::AudioBuffer<float>& buffer,
                               double sampleRate)
{
    VocalProfile profile;

    const int numSamples = buffer.getNumSamples();
    if (numSamples < fft.getSize())
        return profile;

    // 取左声道做分析
    const float* data = buffer.getReadPointer(0);
    juce::AudioBuffer<float> fftBuffer(1, fft.getSize());
    fftBuffer.copyFrom(0, 0, data, fft.getSize());
    fftBuffer.applyGain(fftWindow, 0, fft.getSize(), 1.0f);

    // 执行 FFT
    fft.performFrequencyOnlyForwardTransform(
        fftBuffer.getWritePointer(0)
    );

    // 简单频段能量统计
    const int lowBin    = 0;
    const int midBin    = int(4000.0 / (sampleRate / 2) * fft.getSize());
    const int highBin   = int(10000.0 / (sampleRate / 2) * fft.getSize());

    float lowSum = 0.0f, midSum = 0.0f, highSum = 0.0f;

    for (int i = 0; i < fft.getSize() / 2; ++i)
    {
        float mag = fftBuffer.getSample(0, i);
        if (i < midBin)
            lowSum += mag;
        else if (i < highBin)
            midSum += mag;
        else
            highSum += mag;
    }

    profile.lowDb       = juce::Decibels::gainToDecibels(lowSum);
    profile.presenceDb  = juce::Decibels::gainToDecibels(midSum);
    profile.airDb       = juce::Decibels::gainToDecibels(highSum);

    // 简单齿音估计（高频占比）
    profile.deEss = highSum / (lowSum + midSum + 1e-6f);

    // 简单压缩建议
    profile.compGain = juce::jlimit(0.0f, 12.0f,
        (midSum + highSum) / (lowSum + 1e-6f) * 2.0f);

    return profile;
}
