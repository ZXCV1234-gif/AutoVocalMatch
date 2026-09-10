#pragma once

// 插件里所有可调参数的ID
namespace Param
{
    inline const char* HpfHz         = "hpf_hz";        // 高通滤波
    inline const char* PresenceGain  = "presence_gain"; // 中高频
    inline const char* AirGain       = "air_gain";      // 空气感
    inline const char* LowGain       = "low_gain";      // 低频
    inline const char* CompThresh    = "comp_thresh";   // 压缩阈值
    inline const char* CompRatio     = "comp_ratio";    // 压缩比例
    inline const char* DeEssAmount   = "deess_amount";  // 齿音消除
    inline const char* ReverbDecay   = "reverb_decay";  // 混响衰减
    inline const char* ReverbWet     = "reverb_wet";    // 混响干湿比
    inline const char* MatchStrength = "match_strength";// 匹配强度
}
