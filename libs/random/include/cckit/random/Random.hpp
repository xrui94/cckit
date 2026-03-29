// Random.hpp - CCKIT 随机数库 C++ 接口
//
// 提供现代 C++ 风格的随机数生成功能
// 支持多种随机数引擎和分布类型
//
// 设计原则：
// 1. 完整的 C++ 类型安全接口
// 2. 支持多种随机数引擎（MT19937, MT19937_64, minstd_rand, knuth_b）
// 3. 支持多种分布类型（均匀分布、正态分布、伯努利分布、离散分布）
// 4. 线程安全的全局随机数生成器
// 5. 零开销抽象

#pragma once

// 1. 首先包含配置
#include "RandomConfig.h"

// 2. C 接口
#include "capi/types.h"
#include "capi/rng.h"

// 3. C++ 类型定义
#include "types/RandomGenerator.hpp"

// 4. 兼容层（提供与旧版相同的 API）
#include "RandomCompat.hpp"
