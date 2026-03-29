// RandomGenerator.hpp - 随机数生成器 C++ 接口
//
// 这是 CCKIT 随机数库的 C++ 接口层，提供现代 C++ 风格的随机数生成
//
// 设计原则：
// 1. 提供类型安全的随机数生成接口
// 2. 支持多种随机数引擎
// 3. 支持多种分布类型
// 4. 零开销抽象，内联函数

#pragma once

#include "../capi/types.h"
#include "../capi/rng.h"

#include <vector>
#include <random>
#include <type_traits>
#include <cstdint>

namespace cckit::random
{

    // ========================================
    // RandomGenerator - 随机数生成器
    // ========================================

    class RandomGenerator
    {
    public:
        // 构造函数
        explicit RandomGenerator(cckit_rng_type_t type = CCKIT_RNG_MT19937)
            : m_handle(cckit_rng_create(type)) {}

        explicit RandomGenerator(cckit_rng_type_t type, uint64_t seed)
            : m_handle(cckit_rng_create_with_seed(type, seed)) {}

        // 禁止拷贝
        RandomGenerator(const RandomGenerator&) = delete;
        RandomGenerator& operator=(const RandomGenerator&) = delete;

        // 允许移动
        RandomGenerator(RandomGenerator&& other) noexcept : m_handle(other.m_handle)
        {
            other.m_handle = nullptr;
        }

        RandomGenerator& operator=(RandomGenerator&& other) noexcept
        {
            if (this != &other) {
                if (m_handle) {
                    cckit_rng_destroy(m_handle);
                }
                m_handle = other.m_handle;
                other.m_handle = nullptr;
            }
            return *this;
        }

        // 析构函数
        ~RandomGenerator()
        {
            if (m_handle) {
                cckit_rng_destroy(m_handle);
            }
        }

        // 重新设置种子
        void seed(uint64_t s)
        {
            cckit_rng_seed(m_handle, s);
        }

        // ========================================
        // 整数随机数
        // ========================================

        // 生成 [min, max] 范围内的随机整数
        int32_t nextInt(int32_t min, int32_t max)
        {
            return cckit_rng_int(m_handle, min, max);
        }

        // 生成 [0, max] 范围内的随机整数
        int32_t nextInt(int32_t max)
        {
            return cckit_rng_int(m_handle, 0, max);
        }

        // 生成 [min, max] 范围内的随机无符号整数
        uint32_t nextUInt(uint32_t min, uint32_t max)
        {
            return cckit_rng_uint(m_handle, min, max);
        }

        // 生成 [0, max] 范围内的随机无符号整数
        uint32_t nextUInt(uint32_t max)
        {
            return cckit_rng_uint(m_handle, 0, max);
        }

        // 生成 [min, max] 范围内的随机 64 位整数
        int64_t nextInt64(int64_t min, int64_t max)
        {
            return cckit_rng_int64(m_handle, min, max);
        }

        // 生成 [min, max] 范围内的随机 64 位无符号整数
        uint64_t nextUInt64(uint64_t min, uint64_t max)
        {
            return cckit_rng_uint64(m_handle, min, max);
        }

        // ========================================
        // 浮点随机数
        // ========================================

        // 生成 [min, max] 范围内的随机浮点数
        float nextFloat(float min, float max)
        {
            return cckit_rng_float(m_handle, min, max);
        }

        // 生成 [0, max] 范围内的随机浮点数
        float nextFloat(float max)
        {
            return cckit_rng_float(m_handle, 0.0f, max);
        }

        // 生成 [0, 1) 范围内的随机浮点数
        float nextUnitFloat()
        {
            return cckit_rng_unit_float(m_handle);
        }

        // 生成 [min, max] 范围内的随机双精度浮点数
        double nextDouble(double min, double max)
        {
            return cckit_rng_double(m_handle, min, max);
        }

        // 生成 [0, max] 范围内的随机双精度浮点数
        double nextDouble(double max)
        {
            return cckit_rng_double(m_handle, 0.0, max);
        }

        // 生成 [0, 1) 范围内的随机双精度浮点数
        double nextUnitDouble()
        {
            return cckit_rng_unit_double(m_handle);
        }

        // ========================================
        // 正态分布
        // ========================================

        // 生成正态分布的随机浮点数
        float nextNormal(float mean, float stddev)
        {
            return cckit_rng_normal_float(m_handle, mean, stddev);
        }

        // 生成正态分布的随机双精度浮点数
        double nextNormal(double mean, double stddev)
        {
            return cckit_rng_normal_double(m_handle, mean, stddev);
        }

        // ========================================
        // 布尔值
        // ========================================

        // 生成随机布尔值（50% 概率）
        bool nextBool()
        {
            return cckit_rng_bool(m_handle);
        }

        // 生成随机布尔值（指定概率 p 为 true）
        bool nextBool(float p)
        {
            return cckit_rng_bool_with_prob(m_handle, p);
        }

        // ========================================
        // 加权随机
        // ========================================

        // 根据权重数组选择索引
        size_t weightedIndex(const std::vector<float>& weights)
        {
            if (weights.empty()) return 0;
            return cckit_rng_weighted_index(m_handle, weights.data(), weights.size());
        }

        // 根据权重数组选择索引（双精度版本）
        size_t weightedIndex(const std::vector<double>& weights)
        {
            if (weights.empty()) return 0;
            return cckit_rng_weighted_index_d(m_handle, weights.data(), weights.size());
        }

        // 根据权重选择元素
        template<typename T>
        T weightedChoice(const std::vector<T>& items, const std::vector<float>& weights)
        {
            if (items.empty()) return T{};
            size_t idx = weightedIndex(weights);
            return items[idx];
        }

        // 随机选择元素
        template<typename T>
        T choice(const std::vector<T>& items)
        {
            if (items.empty()) return T{};
            size_t idx = static_cast<size_t>(nextInt(0, static_cast<int32_t>(items.size() - 1)));
            return items[idx];
        }

        // ========================================
        // 范围随机
        // ========================================

        // 打乱数组顺序（Fisher-Yates 洗牌算法）
        template<typename T>
        void shuffle(std::vector<T>& items)
        {
            if (items.size() <= 1) {
                return;  // 空数组或单元素数组无需洗牌
            }
            for (size_t i = items.size() - 1; i > 0; --i) {
                size_t j = static_cast<size_t>(nextUInt(0, static_cast<uint32_t>(i)));
                std::swap(items[i], items[j]);
            }
        }

        // 获取原始句柄
        cckit_rng_t* handle() const { return m_handle; }

    private:
        cckit_rng_t* m_handle;
    };

    // ========================================
    // 全局随机数生成器（便捷接口）
    // ========================================

    // 获取全局随机数生成器（线程安全）
    inline RandomGenerator& globalRng()
    {
        static thread_local RandomGenerator rng(CCKIT_RNG_MT19937);
        return rng;
    }

    // 便捷函数
    inline int32_t randInt(int32_t min, int32_t max)
    {
        return cckit_rand_int(min, max);
    }

    inline float randFloat(float min, float max)
    {
        return cckit_rand_float(min, max);
    }

    inline float randUnitFloat()
    {
        return cckit_rand_unit_float();
    }

} // namespace cckit::random
