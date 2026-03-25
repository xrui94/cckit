// Math.hpp - CCKIT 数学库 C++ 接口
//
// 提供完整的模板化数学类型，对标 CDTMath
// 支持多种精度（float, double）和向量数组类型
//
// 设计原则：
// 1. 完整的模板化类型（Vec2<T>, Vec3<T>, Vec4<T>, Mat3<T>, Mat4<T>, Quat<T>）
// 2. 支持数组类型（Vec2Array<T>, Vec3Array<T>, Vec4Array<T>）
// 3. 提供数学常量和工具函数
// 4. 保持与 CDTMath 兼容的接口设计

#pragma once

// 1. 首先包含配置，确保宏生效 (虽然各个子头文件也会包含，但在这里包含确保顺序)
#include "MathConfig.h"

// 2. 基础工具 (常量和函数)
#include "Constants.hpp"
#include "Utils.hpp"

// 3. 类型定义
#include "types/Vec.hpp"
#include "types/VecArray.hpp"
#include "types/Mat.hpp"
#include "types/Quat.hpp"
