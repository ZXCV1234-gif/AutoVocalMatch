#include "MatchEngine.h"
#include "ParamIds.h"
#include <juce_dsp/juce_dsp.h>

MatchEngine::MatchEngine()
{
}

void MatchEngine::setReference(const VocalProfile& ref)
{
    reference = ref;
}

void MatchEngine::compute(const VocalProfile& current,
                          float strength,
                          float params[9])
{
    // strength: 0~1，控制匹配强度
    // params 顺序对应 ParamIds.h

    // HPF：根据参考歌低频差动态调整
    float lowDiff = reference.lowDb - current.lowDb;
    params[0] = juce::jlimit(20.0f, 200.0f,
        80.0f + lowDiff * 30.0f * strength);

    // 中高频（Presence）
    float midDiff = reference.presenceDb - current.presenceDb;
    params[1] = juce::jlimit(-12.0f, 12.0f,
        midDiff * strength);

    // 空气感（Air）
    float airDiff = reference.airDb - current.airDb;
    params[2] = juce::jlimit(-12.0f, 12.0f,
        airDiff * strength);

    // 低频增益
    params[3] = juce::jlimit(-12.0f, 12.0f,
        lowDiff * 0.5f * strength);

    // 压缩阈值
    float compNeed = reference.compGain - current.compGain;
    params[4] = juce::jlimit(-40.0f, 0.0f,
        -18.0f - compNeed * strength);

    // 压缩比例
    params[5] = juce::jlimit(1.5f, 8.0f,
        2.0f + compNeed * 0.3f * strength);

    // 齿音消除
    float deessNeed = reference.deEss - current.deEss;
    params[6] = juce::jlimit(0.0f, 1.0f,
        deessNeed * strength);

    // 混响衰减
    params[7] = juce::jlimit(0.1f, 5.0f,
        1.5f);

    // 混响干湿比
    params[8] = juce::jlimit(0.0f, 0.5f,
        0.15f * strength);
}
