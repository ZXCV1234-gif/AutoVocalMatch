#include "PluginEditor.h"
#include "PluginProcessor.h"

AutoVocalMatchEditor::AutoVocalMatchEditor(AutoVocalMatchProcessor& p)
    : AudioProcessorEditor(p),
      processor(p)
{
    setSize(500, 360);

    addAndMakeVisible(hpfSlider);
    addAndMakeVisible(lowSlider);
    addAndMakeVisible(presenceSlider);
    addAndMakeVisible(airSlider);
    addAndMakeVisible(compThreshSlider);
    addAndMakeVisible(compRatioSlider);
    addAndMakeVisible(deEssSlider);
    addAndMakeVisible(reverbDecaySlider);
    addAndMakeVisible(reverbWetSlider);
    addAndMakeVisible(strengthSlider);

    hpfSlider.setRange(20.0, 200.0, 1.0);
    lowSlider.setRange(-12.0, 12.0, 0.1);
    presenceSlider.setRange(-12.0, 12.0, 0.1);
    airSlider.setRange(-12.0, 12.0, 0.1);
    compThreshSlider.setRange(-40.0, 0.0, 0.1);
    compRatioSlider.setRange(1.5, 8.0, 0.1);
    deEssSlider.setRange(0.0, 1.0, 0.01);
    reverbDecaySlider.setRange(0.1, 5.0, 0.1);
    reverbWetSlider.setRange(0.0, 0.5, 0.01);
    strengthSlider.setRange(0.0, 1.0, 0.01);

    hpfAttach = std::make_unique<
        juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, Param::HpfHz, hpfSlider);

    lowAttach = std::make_unique<
        juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, Param::LowGain, lowSlider);

    presenceAttach = std::make_unique<
        juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, Param::PresenceGain, presenceSlider);

    airAttach = std::make_unique<
        juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, Param::AirGain, airSlider);

    compThreshAttach = std::make_unique<
        juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, Param::CompThresh, compThreshSlider);

    compRatioAttach = std::make_unique<
        juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, Param::CompRatio, compRatioSlider);

    deEssAttach = std::make_unique<
        juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, Param::DeEssAmount, deEssSlider);

    reverbDecayAttach = std::make_unique<
        juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, Param::ReverbDecay, reverbDecaySlider);

    reverbWetAttach = std::make_unique<
        juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, Param::ReverbWet, reverbWetSlider);

    strengthAttach = std::make_unique<
        juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, Param::MatchStrength, strengthSlider);

    addAndMakeVisible(loadReferenceButton);
    loadReferenceButton.setButtonText("加载参考歌");
    loadReferenceButton.onClick = [this]()
    {
        juce::FileChooser chooser("选择参考人声",
                                  juce::File(),
                                  "*.wav;*.mp3");
        if (chooser.browseForFileToOpen())
            processor.loadReference(chooser.getResult());
    };
}

AutoVocalMatchEditor::~AutoVocalMatchEditor()
{
}

void AutoVocalMatchEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff2b2b2b));
    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("Auto Vocal Match",
               0, 10, getWidth(), 24,
               juce::Justification::centred);
}

void AutoVocalMatchEditor::resized()
{
    int y = 50;
    int h = 24;
    int gap = 30;

    hpfSlider.setBounds(20, y, 460, h
