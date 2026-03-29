// RandomCompat.hpp - 兼容层，提供与旧版相同的 API
//
// 此文件提供与 cdt::common::random 命名空间相同的 API
// 内部使用新的 cckit::random 实现

#pragma once

#include "Random.hpp"
#include <vector>
#include <random>
#include <chrono>

namespace cdt::common::random
{

    // 获取全局随机引擎（兼容旧版 API）
    inline std::mt19937& getRandomEngine()
    {
        static thread_local std::mt19937 engine = []() -> std::mt19937 {
            std::random_device rd;
            auto now = std::chrono::high_resolution_clock::now();
            auto epoch = now.time_since_epoch();
            auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();
            std::seed_seq seq{
                rd(),
                static_cast<unsigned int>(value & 0xFFFFFFFF),
                static_cast<unsigned int>((value >> 32) & 0xFFFFFFFF)
            };
            return std::mt19937{ seq };
        }();
        return engine;
    }

    // 加权随机选择模板函数（兼容旧版 API）
    template<typename T>
    T weightedRandomChoice(
        const std::vector<T>& items,
        const std::vector<float>& weights)
    {
        if (items.empty()) return T{};
        std::discrete_distribution<int> dist(weights.begin(), weights.end());
        return items[dist(getRandomEngine())];
    }

    // 加权随机索引（兼容旧版 API）
    inline size_t weightedRandomIndex(const std::vector<float>& weights)
    {
        if (weights.empty()) return 0;
        std::discrete_distribution<size_t> dist(weights.begin(), weights.end());
        return static_cast<size_t>(dist(getRandomEngine()));
    }

    // 普通随机选择模板函数（兼容旧版 API）
    template<typename T>
    T randomChoice(const std::vector<T>& items)
    {
        if (items.empty()) return T{};
        std::uniform_int_distribution<int> dist(0, static_cast<int>(items.size() - 1));
        return items[dist(getRandomEngine())];
    }

} // namespace cdt::common::random
