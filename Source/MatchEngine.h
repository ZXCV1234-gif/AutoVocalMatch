#pragma once

#include "Analyzer.h"

// 匹配引擎：根据参考歌和当前人声，计算插件参数
class MatchEngine
{
public:
    MatchEngine();

    // 设置参考歌分析结果
    void setReference(const VocalProfile& ref);

    // 根据当前人声，计算匹配参数
    void compute(const VocalProfile& current,
                 float strength, // 0~1，匹配强度
                 float params[9] // 输出参数数组
                );

private:
    VocalProfile reference;
};
