#pragma once

#include "Vec2.hpp"

#include <vector>

namespace cckit::math
{
    using Vec2Array = std::vector<Vec2>;

    // ———————— 数组别名（不再继承 vector） ————————
    using Vec2fArray = std::vector<Vec2f>;
    using Vec2dArray = std::vector<Vec2d>;

} // namespace cckit::math
