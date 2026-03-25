#pragma once

#include "Vec3.hpp"

#include <vector>

namespace cckit::math
{
    using Vec3Array = std::vector<Vec3>;

    // ———————— 数组别名（不再继承 vector） ————————
    using Vec3fArray = std::vector<Vec3f>;
    using Vec3dArray = std::vector<Vec3d>;

} // namespace cckit::math
