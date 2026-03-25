# cckit 基本使用

### 2.1 字符串工具

高级字符串操作，支持中文编码：

**功能特性：**
- UTF-8、GBK、GB2312、GB18030 编码转换
- 宽字符支持（std::wstring）
- 字符串分割、修剪、大小写转换
- UTF-8 验证

**示例：**
```cpp
#include <cckit/string/string.hpp>

using namespace cckit::string;

std::string utf8Text = "你好，世界！";
std::string gbkText = toGbk(utf8Text);
std::wstring wideText = utf8ToWchars(utf8Text);
```

### 2.2 文件系统工具

跨平台的文件和目录操作：

**功能特性：**
- 路径操作和规范化
- 文件操作（复制、删除、获取大小）
- 目录操作（创建、删除、列举）

**示例：**
```cpp
#include <cckit/fs/fs.hpp>

using namespace cckit::fs;

bool exists = exists("/path/to/file");
std::string absolutePath = getAbsolutePath("relative/path");
```

### 2.3 数学模块

完整的数学库，支持 `float` 和 `double` 精度的模板化实现：

**功能特性：**
- 向量：Vec2、Vec3、Vec4，支持完整运算符
- 矩阵：Mat3、Mat4，支持求逆、行列式和变换
- 四元数：旋转、插值和矩阵转换
- 向量数组：Vec2Array、Vec3Array、Vec4Array
- 数学常量和工具函数
- 输出流支持，便于调试

**示例：**
```cpp
#include <cckit/math/Math.hpp>

using namespace cckit::math;

Vec3f position(1.0f, 2.0f, 3.0f);
Mat4f rotation = Mat4f::fromRotation(radians(45.0f), Vec3f::unitY());
Mat4f translation = Mat4f::fromTranslation(Vec3f(10.0f, 0.0f, 0.0f));
Mat4f transform = translation * rotation;

Vec3f result = transform * position;
std::cout << "变换后: " << result << std::endl;
```

### 2.4 日志系统

灵活且高性能的日志记录：

**功能特性：**
- 多种日志级别（Trace、Debug、Info、Warn、Error、Critical）
- 自定义回调和用户上下文
- 源码位置跟踪（文件、行号、函数名）
- 编译期日志级别过滤
- Fallback 机制

**示例：**
```cpp
#include <cckit/logging/logging.hpp>

using namespace cckit::log;

initializeLogging();
setLogCallback([](LogLevel level, const SourceLoc& loc, 
                  const std::string& msg, void* ctx) {
    printf("[%s:%d] %s\n", loc.filename, loc.line, msg.c_str());
});

CCKIT_LOG_INFO("应用程序已启动");
CCKIT_LOG_ERROR("错误代码: {}", error_code);
```

### 2.5 UUID 模块

符合 RFC 4122 的 UUID v4 生成：

**功能特性：**
- 随机 UUID 生成
- 字符串转换
- 比较和哈希
- std::unordered_map 支持

**示例：**
```cpp
#include <cckit/uuid/uuid.hpp>

using namespace cckit;

Uuid id = Uuid::generate();
std::string idStr = id.toString();
bool isNil = id.isNil();
```