#pragma once

#include <vector>
#include <span>

namespace knn {
    using FeatureVector = std::vector<double>;
    using FeatureView = std::span<const double>;

}
