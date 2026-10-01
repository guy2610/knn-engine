#pragma once

#include <span>
#include <vector>

namespace knn {
using FeatureVector = std::vector<double>;
using FeatureView = std::span<const double>;

} // namespace knn
