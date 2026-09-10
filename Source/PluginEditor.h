#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

class AutoVocalMatchEditor : public juce::AudioProcessorEditor
{
public:
    explicit AutoVocalMatchEditor(AutoVocalMatchProcessor&);
    ~AutoVocalMatchEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AutoVocalMatchProcessor& processor;

    juce::Slider hpfSlider;
    juce::Slider lowSlider;
    juce::Slider presenceSlider;
    juce::Slider airSlider;
    juce::Slider compThreshSlider;
    juce::Slider compRatioSlider;
    juce::Slider deEssSlider;
    juce::Slider reverbDecaySlider;
    juce::Slider reverbWetSlider;
    juce::Slider strengthSlider;

    juce::TextButton loadReferenceButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        hpfAttach, lowAttach, presenceAttach, airAttach,
        compThreshAttach, compRatioAttach, deEssAttach,
        reverbDecayAttach, reverbWetAttach, strengthAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AutoVocalMatchEditor)
};
