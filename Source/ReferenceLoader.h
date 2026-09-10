#pragma once

#include "Analyzer.h"

// 参考歌加载器：加载音频文件并分析人声特征
class ReferenceLoader
{
public:
    ReferenceLoader();

    // 加载参考音频文件（WAV / MP3）
    bool load(const juce::File& file);

    // 获取分析结果
    const VocalProfile& getProfile() const;

private:
    VocalProfile profile;
};
