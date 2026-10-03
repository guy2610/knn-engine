#pragma once
#include "knn/features.hpp"

#include <optional>

namespace knn {
enum class DistanceMetric {
    Euclidean,
    Manhattan,
    Chebyshev,
    Canberra,
    Minkowski,
};

struct DistanceConfig {
    DistanceMetric metric;
    std::optional<double> minkowski_p;
};

[[nodiscard]] double compute_distance(FeatureView lhs, FeatureView rhs, DistanceConfig config);

} // namespace knn
