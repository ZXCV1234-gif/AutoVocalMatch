#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

// 人声分析结果结构体
struct VocalProfile
{
    float lowDb = 0.0f;       // 低频能量
    float presenceDb = 0.0f;  // 中高频（存在感）
    float airDb = 0.0f;       // 空气感
    float deEss = 0.0f;       // 齿音强度
    float compGain = 0.0f;    // 建议压缩量
};

// 分析器：从音频块中提取人声特征
class Analyzer
{
public:
    Analyzer();

    // 分析一帧音频
    VocalProfile analyze(const juce::AudioBuffer<float>& buffer,
                         double sampleRate);

private:
    juce::dsp::FFT fft;
    juce::AudioBuffer<float> fftWindow;
};
