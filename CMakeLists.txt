cmake_minimum_required(VERSION 3.22)

project(AutoVocalMatch VERSION 0.1.0)

# JUCE 子模块
add_subdirectory(JUCE)

# 创建插件
juce_add_plugin(AutoVocalMatch
    COMPANY_NAME "ZXCV"
    PLUGIN_MANUFACTURER_CODE "ZXCV"
    PLUGIN_CODE "AVM1"
    FORMATS VST3
    PRODUCT_NAME "Auto VocalMatch"
)

# 源文件（你现有的 13 个文件）
target_sources(AutoVocalMatch PRIVATE
    Source/ParamIds.h
    Source/Analyzer.h
    Source/Analyzer.cpp
    Source/MatchEngine.h
    Source/MatchEngine.cpp
    Source/DspChain.h
    Source/DspChain.cpp
    Source/ReferenceLoader.h
    Source/ReferenceLoader.cpp
    Source/PluginProcessor.h
    Source/PluginProcessor.cpp
    Source/PluginEditor.h
    Source/PluginEditor.cpp
)

# 头文件搜索路径
target_include_directories(AutoVocalMatch PRIVATE Source)

# 链接 JUCE 模块
target_link_libraries(AutoVocalMatch
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
)

# C++ 标准
target_compile_features(AutoVocalMatch PRIVATE cxx_std_20)
