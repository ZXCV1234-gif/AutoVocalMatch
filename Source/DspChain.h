#pragma once

#include <juce_dsp/juce_dsp.h>

// DSP 处理链：HPF + EQ + Comp + DeEss + Reverb
class DspChain
{
public:
    DspChain();

    // 准备播放
    void prepare(double sampleRate, int blockSize);

    // 处理音频
    void process(juce::AudioBuffer<float>& buffer);

    // 设置参数
    void setHpfHz(float hz);
    void setEqGains(float low, float presence, float air);
    void setCompressor(float threshDb, float ratio);
    void setDeEss(float amount);
    void setReverb(float decay, float wet);

private:
    juce::dsp::ProcessorChain<
        juce::dsp::IIR::Filter<float>,   // HPF
        juce::dsp::EQ::Filter<float>,    // Low shelf
        juce::dsp::EQ::Filter<float>,    // Presence peak
        juce::dsp::EQ::Filter<float>,    // Air high shelf
        juce::dsp::Compressor<float>,
        juce::dsp::DeEsser<float>,
        juce::dsp::Reverb
    > chain;

    double currentSampleRate = 44100.0;
};
