// Vec.hpp - 向量的公共定义和工具
//
// 这是 CCKIT 数学库的向量公共接口层，提供：
// 1. Vec2、Vec3、Vec4相关的公共定义
// 2. 向量工具函数（如果需要）
//
// 具体的矩阵类实现请参考：
// - Vec2.hpp: 1x2 向量
// - Vec3.hpp: 1x2 向量
// - Vec4.hpp: 1x4 向量

#pragma once

// 首先包含配置，确保宏生效 (虽然各个子头文件也会包含，但在这里包含确保顺序)
#include "../MathConfig.h"

// 包含具体的矩阵实现
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"

namespace cckit::math
{

// ========================================
// 向量工具函数（如果需要）
// ========================================

} // namespace cckit::math
