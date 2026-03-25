# CCKIT 

- 设计白皮书：构建永恒的 C++ 基础设施

## 1. 核心理念 (Core Philosophy)

> "Decouple the Interface from the Implementation, and the ABI from the Compiler."
(将接口与实现解耦，将二进制接口 (ABI) 与编译器解耦。)

在 C++ 世界中，编译器版本、标准库实现、编译选项的微小差异都可能导致二进制不兼容。CCKIT 的核心思想是引入一层“中间表示层 (IR)”：

- 对内 (Implementation)：拥抱变化。可以自由使用 GLM, Eigen, nlohmann/json, spdlog 等任何现代库，甚至随时替换它们。
- 对外 (Interface)：坚守不变。暴露纯 C 风格的、基于 POD (Plain Old Data) 的稳定接口。
- 对用户 (Experience)：提供现代 C++ 的优雅体验。通过轻量级 Wrapper 让用户感觉不到底层的 C 风格，享受命名空间、运算符重载和类型安全。

## 2. 四大设计原则 (The Four Pillars)

### 🏛️ 原则一：绝对的二进制稳定性 (Absolute ABI Stability)

- 定义：无论底层实现如何重构、后端库如何升级、编译器如何更新，只要 cckit 的版本号主版本不变，用户的程序无需重新编译即可链接新的动态库。
- 手段：
  - 公开头文件中严禁出现 std:: 容器、虚函数、继承链、模板实例化。
  - 所有公开数据类型必须是 POD (Plain Old Data) 结构体。
  - 所有公开函数必须遵循 extern "C" linkage，使用 C 调用约定。

### ⚡ 原则二：零开销抽象 (Zero-Overhead Abstraction)

- 定义：用户使用的现代 C++ 封装层（Wrapper），便于 C++ 调用的同时，不应带来额外的运行时性能损耗。
- 手段：
  - Wrapper 类内部直接持有 POD 数据（值语义），而非指针（避免堆分配）。
  - Wrapper 的成员函数必须是 inline 或编译器可内联的，直接映射到底层 C API。
  -对于简单运算（如向量加法），Wrapper 可直接内联计算，甚至绕过 C API 调用以极致优化。

### 🔌 原则三：后端可插拔性 (Pluggable Backends)

- 定义：核心逻辑与具体算法实现完全隔离。同一个接口，可以在编译时或运行时切换不同的底层引擎。
- 手段：
  - 定义统一的 IR 接口规范（如 cckit_vec3_add）。
  - 实现多个 适配器 (Adapters)：adapter_glm.cpp, adapter_eigen.cpp, adapter_neon.cpp。
  - 通过构建系统（CMake Option）或运行时配置决定链接哪个适配器。

### 🛡️ 原则四：语言无关的互操作性 (Language Agnostic Interop)
- 定义：由于底层是纯 C + POD，CCKIT 天然可以被其他语言调用。
- 手段：
  - Python (ctypes/pybind11), C#, Rust, Go 都可以直接理解 POD 结构体和 C 函数。
  - 这使得 CCKIT 不仅服务于 C++，还能成为跨语言项目的通用数据交换格式。


## 3. 实战演练：CCKIT 数学库 (cckit-math)
让我们看看这些原则如何落地到一个具体的 3D 数学库 中。

### 第一层：IR 定义层 (The IR Layer)

**文件**: include/cckit/math/types.h
**角色**: 契约。这是唯一不可轻易变更的部分。
```c++
#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// 【原则一】纯粹的 POD 结构体
// 内存布局：连续 12 字节 (float x, y, z)
// 在任何编译器、任何平台、任何时代，这都是 12 字节。
typedef struct {
    float x;
    float y;
    float z;
} cckit_vec3_t;

// 【原则一】纯 C 接口
// 无命名空间，无类，无异常，无模板
// 参数通过指针传递，保证大结构体效率
void cckit_vec3_add(cckit_vec3_t* out, const cckit_vec3_t* a, const cckit_vec3_t* b);
void cckit_vec3_normalize(cckit_vec3_t* v);
float cckit_vec3_length(const cckit_vec3_t* v);

#ifdef __cplusplus
}
#endif

// 编译期检查：确保它是 POD，如果不是则编译失败
#ifdef __cplusplus
static_assert(std::is_trivial<cckit_vec3_t>::value, "cckit_vec3_t must be trivial");
static_assert(std::is_standard_layout<cckit_vec3_t>::value, "cckit_vec3_t must be standard layout");
#endif
```

### 第二层：后端适配层 (The Adapter Layer)

**文件**: src/math/backend_glm.cpp (示例：使用 GLM 后端)
**角色**: 实现者。这里可以随意使用任何黑科技。

```c++
#include "cckit/math/types.h"
#include <glm/glm.hpp> // 依赖第三方库！只有这里知道 GLM 的存在

// 实现符合 IR 规范的函数
void cckit_vec3_add(cckit_vec3_t* out, const cckit_vec3_t* a, const cckit_vec3_t* b) {
    // 1. 映射：从 POD 到 GLM 对象 (栈上拷贝，极快)
    glm::vec3 v_a(a->x, a->y, a->z);
    glm::vec3 v_b(b->x, b->y, b->z);
    
    // 2. 计算：使用 GLM 的强大功能
    glm::vec3 v_res = v_a + v_b;
    
    // 3. 写回：从 GLM 对象到 POD
    out->x = v_res.x;
    out->y = v_res.y;
    out->z = v_res.z;
}

void cckit_vec3_normalize(cckit_vec3_t* v) {
    glm::vec3 temp(v->x, v->y, v->z);
    temp = glm::normalize(temp);
    v->x = temp.x;
    v->y = temp.y;
    v->z = temp.z;
}
```
(注：你可以轻松创建 backend_eigen.cpp 或 backend_simd.cpp，只需实现相同的 cckit_ 函数签名)

### 第三层：C++ 包装层 (The C++ Wrapper Layer)

**文件**: include/cckit/math.hpp
**角色**: 体验官。为用户提供现代 C++ 体验。

```c++
#pragma once
#include "cckit/math/types.h" // 包含底层 C 接口
#include <cmath>

namespace cckit {

    // 【原则二】值语义类，内部直接持有 POD
    class Vec3 {
    private:
        cckit_vec3_t _data; 

    public:
        // 构造函数
        constexpr Vec3() : _data{0.f, 0.f, 0.f} {}
        constexpr Vec3(float x, float y, float z) : _data{x, y, z} {}

        // 【原则二】内联成员函数，直接调用底层 C API
        inline void normalize() {
            cckit_vec3_normalize(&_data);
        }

        inline float length() const {
            return cckit_vec3_length(&_data);
        }

        // 【原则二】运算符重载，提供语法糖
        inline Vec3 operator+(const Vec3& other) const {
            cckit_vec3_t res;
            cckit_vec3_add(&res, &_data, &other._data);
            return Vec3(res.x, res.y, res.z);
        }

        inline Vec3 operator*(float s) const {
            // 极致优化：简单标量乘法直接内联计算，甚至不调 C API
            return Vec3(_data.x * s, _data.y * s, _data.z * s);
        }

        // 访问器
        inline float& x() { return _data.x; }
        inline float& y() { return _data.y; }
        inline float& z() { return _data.z; }
        
        // 隐式转换：允许无缝传递给需要底层 POD 的 C API
        inline operator const cckit_vec3_t&() const { return _data; }
        inline operator cckit_vec3_t&() { return _data; }
    };

} // namespace cckit
```

### 第四层：用户代码 (User Code)

**文件**: main.cpp
**角色**: 受益者。

```c++
#include <cckit/math.hpp>
#include <iostream>

int main() {
    // 看起来就像在使用原生现代 C++ 库
    cckit::Vec3 a(1.0f, 2.0f, 3.0f);
    cckit::Vec3 b(4.0f, 5.0f, 6.0f);

    // 优雅的链式调用
    cckit::Vec3 c = (a + b) * 2.0f;
    c.normalize();

    std::cout << "Result: " << c.x() << ", " << c.y() << ", " << c.z() << std::endl;

    // 如果需要与旧 C 代码交互，直接隐式转换
    // legacy_c_function(c); 
    return 0;
}
```

## 4. 这种设计带来的巨大价值

1. 发布即永恒：
你编译好了 libcckit-math.so (基于 GLM)。一年后，你想换成基于 Eigen 且经过 AVX512 优化的版本。你只需要替换这个 .so 文件。
用户的 main.cpp 不需要重新编译，甚至不需要重新链接（只要符号名没变），程序自动获得性能提升和新特性。

2. 消除“依赖地狱”：
用户项目不需要安装 GLM 或 Eigen。他们只依赖 CCKIT。你可以在后台偷偷把 100MB 的依赖库换成 10KB 的手写 SIMD 代码，用户毫无感知。

3. 跨语言桥梁：
明天你的老板说：“我们要用 Python 写工具脚本调用这个数学库。”
你不需要重写任何东西。Python 的 ctypes 可以直接加载 libcckit-math.so 并操作 cckit_vec3_t 结构体。

4. 测试与模拟：
你可以编写一个 backend_mock.cpp，所有的数学运算都返回固定值或记录日志。在单元测试中链接这个后端，轻松测试业务逻辑而不依赖真实的数学库。

## 5. 总结

CCKIT 不只是一个库的名字，它代表了一种成熟的工业级 C++ 架构思维：
- 用 C 的稳定性 守住底线。
- 用 C++ 的表达能力 提升体验。
- 用 模块化适配 拥抱变化。