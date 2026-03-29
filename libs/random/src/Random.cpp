// Random.cpp - 随机数生成器实现 (std::random 后端)
//
// C 接口通过 std::random 实现
// C++ 用户直接使用 RandomGenerator 类

#include "cckit/random/capi/rng.h"
#include "cckit/export.h"

#include <random>
#include <chrono>
#include <memory>
#include <mutex>
#include <cassert>

namespace
{

    // 随机数引擎包装器
    struct RngEngine
    {
        std::unique_ptr<std::mt19937> mt19937;
        std::unique_ptr<std::mt19937_64> mt19937_64;
        std::unique_ptr<std::minstd_rand> minstd;
        std::unique_ptr<std::knuth_b> knuth_b;
        cckit_rng_type_t type;

        RngEngine(cckit_rng_type_t t, uint64_t seed) : type(t)
        {
            switch (t) {
            case CCKIT_RNG_MT19937:
                mt19937 = std::make_unique<std::mt19937>(static_cast<std::mt19937::result_type>(seed));
                break;
            case CCKIT_RNG_MT19937_64:
                mt19937_64 = std::make_unique<std::mt19937_64>(seed);
                break;
            case CCKIT_RNG_MINSTD_RAND:
                minstd = std::make_unique<std::minstd_rand>(static_cast<std::minstd_rand::result_type>(seed));
                break;
            case CCKIT_RNG_KNUTH_B:
                knuth_b = std::make_unique<std::knuth_b>(static_cast<std::knuth_b::result_type>(seed));
                break;
            default:
                mt19937 = std::make_unique<std::mt19937>(static_cast<std::mt19937::result_type>(seed));
                break;
            }
        }

        void seed(uint64_t s)
        {
            switch (type) {
            case CCKIT_RNG_MT19937:
                mt19937->seed(static_cast<std::mt19937::result_type>(s));
                break;
            case CCKIT_RNG_MT19937_64:
                mt19937_64->seed(s);
                break;
            case CCKIT_RNG_MINSTD_RAND:
                minstd->seed(static_cast<std::minstd_rand::result_type>(s));
                break;
            case CCKIT_RNG_KNUTH_B:
                knuth_b->seed(static_cast<std::knuth_b::result_type>(s));
                break;
            }
        }

        uint64_t next()
        {
            switch (type) {
            case CCKIT_RNG_MT19937:
                return static_cast<uint64_t>((*mt19937)());
            case CCKIT_RNG_MT19937_64:
                return (*mt19937_64)();
            case CCKIT_RNG_MINSTD_RAND:
                return static_cast<uint64_t>((*minstd)());
            case CCKIT_RNG_KNUTH_B:
                return static_cast<uint64_t>((*knuth_b)());
            default:
                return static_cast<uint64_t>((*mt19937)());
            }
        }
    };

    // 生成高质量种子
    uint64_t generateSeed()
    {
        std::random_device rd;
        auto now = std::chrono::high_resolution_clock::now();
        auto epoch = now.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();

        std::seed_seq seq{
            rd(),
            rd(),
            static_cast<uint32_t>(value & 0xFFFFFFFF),
            static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF)
        };

        uint32_t seeds[2];
        seq.generate(seeds, seeds + 2);
        return (static_cast<uint64_t>(seeds[0]) << 32) | seeds[1];
    }

} // anonymous namespace

// cckit_rng_t 的实际定义
struct cckit_rng_t
{
    RngEngine engine;

    cckit_rng_t(cckit_rng_type_t type, uint64_t seed) : engine(type, seed) {}
};

extern "C"
{

// ========================================
// 引擎管理
// ========================================

cckit_rng_t* cckit_rng_create(cckit_rng_type_t type)
{
    return new cckit_rng_t(type, generateSeed());
}

cckit_rng_t* cckit_rng_create_with_seed(cckit_rng_type_t type, uint64_t seed)
{
    return new cckit_rng_t(type, seed);
}

void cckit_rng_destroy(cckit_rng_t* rng)
{
    delete rng;
}

void cckit_rng_seed(cckit_rng_t* rng, uint64_t seed)
{
    if (rng) {
        rng->engine.seed(seed);
    }
}

// ========================================
// 整数随机数
// ========================================

int32_t cckit_rng_int(cckit_rng_t* rng, int32_t min, int32_t max)
{
    assert(rng != nullptr);
    assert(min <= max);
    std::uniform_int_distribution<int32_t> dist(min, max);
    std::mt19937 temp(static_cast<std::mt19937::result_type>(rng->engine.next()));
    return dist(temp);
}

uint32_t cckit_rng_uint(cckit_rng_t* rng, uint32_t min, uint32_t max)
{
    assert(rng != nullptr);
    assert(min <= max);
    std::uniform_int_distribution<uint32_t> dist(min, max);
    std::mt19937 temp(static_cast<std::mt19937::result_type>(rng->engine.next()));
    return dist(temp);
}

int64_t cckit_rng_int64(cckit_rng_t* rng, int64_t min, int64_t max)
{
    assert(rng != nullptr);
    assert(min <= max);
    std::uniform_int_distribution<int64_t> dist(min, max);
    std::mt19937_64 temp(rng->engine.next());
    return dist(temp);
}

uint64_t cckit_rng_uint64(cckit_rng_t* rng, uint64_t min, uint64_t max)
{
    assert(rng != nullptr);
    assert(min <= max);
    std::uniform_int_distribution<uint64_t> dist(min, max);
    std::mt19937_64 temp(rng->engine.next());
    return dist(temp);
}

// ========================================
// 浮点随机数
// ========================================

float cckit_rng_float(cckit_rng_t* rng, float min, float max)
{
    assert(rng != nullptr);
    assert(min <= max);
    std::uniform_real_distribution<float> dist(min, max);
    std::mt19937 temp(static_cast<std::mt19937::result_type>(rng->engine.next()));
    return dist(temp);
}

double cckit_rng_double(cckit_rng_t* rng, double min, double max)
{
    assert(rng != nullptr);
    assert(min <= max);
    std::uniform_real_distribution<double> dist(min, max);
    std::mt19937_64 temp(rng->engine.next());
    return dist(temp);
}

float cckit_rng_unit_float(cckit_rng_t* rng)
{
    return cckit_rng_float(rng, 0.0f, 1.0f);
}

double cckit_rng_unit_double(cckit_rng_t* rng)
{
    return cckit_rng_double(rng, 0.0, 1.0);
}

// ========================================
// 正态分布
// ========================================

float cckit_rng_normal_float(cckit_rng_t* rng, float mean, float stddev)
{
    assert(rng != nullptr);
    std::normal_distribution<float> dist(mean, stddev);
    std::mt19937 temp(static_cast<std::mt19937::result_type>(rng->engine.next()));
    return dist(temp);
}

double cckit_rng_normal_double(cckit_rng_t* rng, double mean, double stddev)
{
    assert(rng != nullptr);
    std::normal_distribution<double> dist(mean, stddev);
    std::mt19937_64 temp(rng->engine.next());
    return dist(temp);
}

// ========================================
// 布尔值
// ========================================

bool cckit_rng_bool(cckit_rng_t* rng)
{
    return cckit_rng_bool_with_prob(rng, 0.5f);
}

bool cckit_rng_bool_with_prob(cckit_rng_t* rng, float p)
{
    assert(rng != nullptr);
    assert(p >= 0.0f && p <= 1.0f);
    std::bernoulli_distribution dist(p);
    std::mt19937 temp(static_cast<std::mt19937::result_type>(rng->engine.next()));
    return dist(temp);
}

// ========================================
// 加权随机
// ========================================

size_t cckit_rng_weighted_index(cckit_rng_t* rng, const float* weights, size_t count)
{
    assert(rng != nullptr);
    assert(weights != nullptr);
    assert(count > 0);

    std::discrete_distribution<size_t> dist(weights, weights + count);
    std::mt19937 temp(static_cast<std::mt19937::result_type>(rng->engine.next()));
    return dist(temp);
}

size_t cckit_rng_weighted_index_d(cckit_rng_t* rng, const double* weights, size_t count)
{
    assert(rng != nullptr);
    assert(weights != nullptr);
    assert(count > 0);

    std::discrete_distribution<size_t> dist(weights, weights + count);
    std::mt19937_64 temp(rng->engine.next());
    return dist(temp);
}

// ========================================
// 全局引擎（便捷接口）
// ========================================

cckit_rng_t* cckit_rng_global()
{
    static thread_local cckit_rng_t* global_rng = nullptr;
    if (!global_rng) {
        global_rng = cckit_rng_create(CCKIT_RNG_MT19937);
    }
    return global_rng;
}

int32_t cckit_rand_int(int32_t min, int32_t max)
{
    return cckit_rng_int(cckit_rng_global(), min, max);
}

float cckit_rand_float(float min, float max)
{
    return cckit_rng_float(cckit_rng_global(), min, max);
}

float cckit_rand_unit_float()
{
    return cckit_rng_unit_float(cckit_rng_global());
}

} // extern "C"
