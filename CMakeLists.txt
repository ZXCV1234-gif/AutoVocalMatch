cmake_minimum_required(VERSION 3.22)
project(AutoVocalMatch VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(JUCE)

juce_add_plugin(AutoVocalMatch
    COMPANY_NAME "VocalLab"
    PLUGIN_CODE Avm1
    FORMATS VST3
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    VST3_CATEGORIES "Fx"
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(AutoVocalMatch PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/ParamIds.h
    Source/Analyzer.cpp
    Source/MatchEngine.cpp
    Source/DspChain.cpp
    Source/ReferenceLoader.cpp
)

target_include_directories(AutoVocalMatch PRIVATE Source)

target_link_libraries(AutoVocalMatch PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_basics
    juce::juce_audio_formats
)
