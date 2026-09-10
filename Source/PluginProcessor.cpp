#include "PluginProcessor.h"
#include "PluginEditor.h"

AutoVocalMatchProcessor::AutoVocalMatchProcessor()
    : AudioProcessor(BusesProperties()
          .withInput("Input", juce::AudioChannelSet::stereo())
          .withOutput("Output", juce::AudioChannelSet::stereo())),
      parameters(*this, nullptr,
          "AutoVocalMatch",
          {
              std::make_unique<juce::AudioParameterFloat>(
                  Param::HpfHz, "HPF Hz",
                  juce::NormalisableRange<float>(20.0f, 200.0f, 1.0f), 80.0f),
              std::make_unique<juce::AudioParameterFloat>(
                  Param::LowGain, "Low Gain",
                  juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f),
              std::make_unique<juce::AudioParameterFloat>(
                  Param::PresenceGain, "Presence",
                  juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f),
              std::make_unique<juce::AudioParameterFloat>(
                  Param::AirGain, "Air",
                  juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f),
              std::make_unique<juce::AudioParameterFloat>(
                  Param::CompThresh, "Comp Thresh",
                  juce::NormalisableRange<float>(-40.0f, 0.0f, 0.1f), -18.0f),
              std::make_unique<juce::AudioParameterFloat>(
                  Param::CompRatio, "Comp Ratio",
                  juce::NormalisableRange<float>(1.5f, 8.0f, 0.1f), 2.0f),
              std::make_unique<juce::AudioParameterFloat>(
                  Param::DeEssAmount, "De-Ess",
                  juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f),
              std::make_unique<juce::AudioParameterFloat>(
                  Param::ReverbDecay, "Reverb Decay",
                  juce::NormalisableRange<float>(0.1f, 5.0f, 0.1f), 1.5f),
              std::make_unique<juce::AudioParameterFloat>(
                  Param::ReverbWet, "Reverb Wet",
                  juce::NormalisableRange<float>(0.0f, 0.5f, 0.01f), 0.15f),
              std::make_unique<juce::AudioParameterFloat>(
                  Param::MatchStrength, "Match Strength",
                  juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f)
          })
{
}

AutoVocalMatchProcessor::~AutoVocalMatchProcessor()
{
}

void AutoVocalMatchProcessor::prepareToPlay(double sampleRate, int blockSize)
{
    dspChain.prepare(sampleRate, blockSize);
}

void AutoVocalMatchProcessor::releaseResources()
{
}

void AutoVocalMatchProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                          juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    // 分析当前人声
    VocalProfile current = analyzer.analyze(buffer, getSampleRate());

    // 自动匹配
    float strength = *parameters.getRawParameterValue(Param::MatchStrength);
    float params[9];
    matchEngine.compute(current, strength, params);

    // 更新 DSP
    dspChain.setHpfHz(params[0]);
    dspChain.setEqGains(params[3], params[1], params[2]);
    dspChain.setCompressor(params[4], params[5]);
    dspChain.setDeEss(params[6]);
    dspChain.setReverb(params[7], params[8]);

    // 处理音频
    dspChain.process(buffer);
}

juce::AudioProcessorEditor* AutoVocalMatchProcessor::createEditor()
{
    return new juce::AudioProcessorEditor(*this);
}

void AutoVocalMatchProcessor::getStateInformation(juce::MemoryBlock& dest)
{
    juce::MemoryOutputStream stream(dest, true);
    parameters.state.writeToStream(stream);
}

void AutoVocalMatchProcessor::setStateInformation(const void* data, int size)
{
    auto tree = juce::ValueTree::readFromData(data, (size_t)size);
    if (tree.isValid())
        parameters.replaceState(tree);
}

bool AutoVocalMatchProcessor::loadReference(const juce::File& file)
{
    if (referenceLoader.load(file))
    {
        matchEngine.setReference(referenceLoader.getProfile());
        return true;
    }
    return false;
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AutoVocalMatch
