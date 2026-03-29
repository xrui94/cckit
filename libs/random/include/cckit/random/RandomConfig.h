// RandomConfig.h - 随机数库配置
#pragma once

// ========================================
// 默认引擎配置
// ========================================

// 默认使用 MT19937 引擎（可通过宏定义修改）
#ifndef CCKIT_RNG_DEFAULT_TYPE
    #define CCKIT_RNG_DEFAULT_TYPE CCKIT_RNG_MT19937
#endif
