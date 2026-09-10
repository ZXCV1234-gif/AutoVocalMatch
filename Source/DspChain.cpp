#include "DspChain.h"

DspChain::DspChain()
{
}

void DspChain::prepare(double sampleRate, int blockSize)
{
    currentSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = blockSize;
    spec.numChannels = 2;

    chain.prepare(spec);

    // 默认参数
    setHpfHz(80.0f);
    setEqGains(0.0f, 0.0f, 0.0f);
    setCompressor(-18.0f, 2.0f);
    setDeEss(0.0f);
    setReverb(1.5f, 0.15f);
}

void DspChain::process(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    chain.process(context);
}

void DspChain::setHpfHz(float hz)
{
    auto& hpf = chain.get<0>();
    hpf.setType(juce::dsp::IIR::FilterType::lowPass);
    hpf.setCutoffFrequencyHz(hz);
    hpf.setQ(0.7f);
}

void DspChain::setEqGains(float low, float presence, float air)
{
    auto& lowShelf = chain.get<1>();
    lowShelf.setType(juce::dsp::EQ::FilterType::lowShelf);
    lowShelf.setGainDecibels(low);
    lowShelf.setCutoffFrequencyHz(120.0f);

    auto& presencePeak = chain.get<2>();
    presencePeak.setType(juce::dsp::EQ::FilterType::peak);
    presencePeak.setGainDecibels(presence);
    presencePeak.setCutoffFrequencyHz(3500.0f);
    presencePeak.setQ(1.2f);

    auto& airShelf = chain.get<3>();
    airShelf.setType(juce::dsp::EQ::FilterType::highShelf);
    airShelf.setGainDecibels(air);
    airShelf.setCutoffFrequencyHz(10000.0f);
}

void DspChain::setCompressor(float threshDb, float ratio)
{
    auto& comp = chain.get<4>();
    comp.setThreshold(threshDb);
    comp.setRatio(ratio);
    comp.setAttack(10.0f);
    comp.setRelease(100.0f);
}

void DspChain::setDeEss(float amount)
{
    auto& deess = chain.get<5>();
    deess.setFrequency(6000.0f);
    deess.setThreshold(-18.0f + amount * 12.0f);
}

void DspChain::setReverb(float decay, float wet)
{
    auto& reverb = chain.get<6>();
    juce::dsp::Reverb::Parameters params;
    params.roomSize = decay / 5.0f;
    params.damping = 0.5f;
    params.wetLevel = wet;
    params.dryLevel = 1.0f - wet;
    params.width = 0.8f;
    params.freezeMode = 0.0f;
    reverb.setParameters(params);
}
