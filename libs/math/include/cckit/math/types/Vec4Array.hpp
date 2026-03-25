#pragma once

#include "Vec4.hpp"

#include <vector>

namespace cckit::math
{
    using Vec4Array = std::vector<Vec4>;

    // ———————— 数组别名（不再继承 vector） ————————
    using Vec4fArray = std::vector<Vec4f>;
    using Vec4dArray = std::vector<Vec4d>;

} // namespace cckit::math
