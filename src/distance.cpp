#include "knn/distance.hpp"

#include "knn/features.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace knn {

namespace {
void validate_inputs(const knn::FeatureView& lhs, const knn::FeatureView& rhs) {
    if (lhs.empty() || rhs.empty()) {
        throw std::invalid_argument("Feature vectors must not be empty");
    }

    if (lhs.size() != rhs.size()) {
        throw std::invalid_argument("Feature vectors must have matching dimensions");
    }
}
double validate_minkowski_config(const DistanceConfig& config) {
    if (!config.minkowski_p.has_value()) {
        throw std::invalid_argument("Minkowski distance requires p");
    }

    const double exponent = config.minkowski_p.value();

    if (!std::isfinite(exponent) || exponent < 1.0) {
        throw std::invalid_argument("Minkowski p must be finite and at least 1");
    }

    return exponent;
}
void validate_config(const knn::DistanceConfig& config) {
    if (config.metric != DistanceMetric::Minkowski && config.minkowski_p.has_value()) {
        throw std::invalid_argument("Minkowski p is only valid for Minkowski distance");
    }
}

double euclidean_distance(knn::FeatureView lhs, knn::FeatureView rhs) {
    double sum{0.0};
    auto lhs_it = lhs.begin();
    auto rhs_it = rhs.begin();

    for (; lhs_it != lhs.end(); ++lhs_it, ++rhs_it) {
        const double difference = *lhs_it - *rhs_it;
        sum += difference * difference;
    }
    return std::sqrt(sum);
}
double manhattan_distance(knn::FeatureView lhs, knn::FeatureView rhs) {
    double sum{0.0};
    auto lhs_it = lhs.begin();
    auto rhs_it = rhs.begin();

    for (; lhs_it != lhs.end(); ++lhs_it, ++rhs_it) {
        const double difference = *lhs_it - *rhs_it;
        sum += std::abs(difference);
    }
    return sum;
}
double chebyshev_distance(knn::FeatureView lhs, knn::FeatureView rhs) {
    double max_distance{0.0};
    auto lhs_it = lhs.begin();
    auto rhs_it = rhs.begin();

    for (; lhs_it != lhs.end(); ++lhs_it, ++rhs_it) {
        const double difference = *lhs_it - *rhs_it;
        max_distance = std::max(max_distance, std::abs(difference));
    }
    return max_distance;
}
double canberra_distance(knn::FeatureView lhs, knn::FeatureView rhs) {
    double sum{0.0};
    auto lhs_it = lhs.begin();
    auto rhs_it = rhs.begin();

    for (; lhs_it != lhs.end(); ++lhs_it, ++rhs_it) {
        const double denominator = std::abs(*lhs_it) + std::abs(*rhs_it);
        if (denominator != 0.0) {
            sum += std::abs(*lhs_it - *rhs_it) / denominator;
        }
    }
    return sum;
}
double minkowski_distance(knn::FeatureView lhs, knn::FeatureView rhs, const double exponent) {
    double sum{0.0};
    auto lhs_it = lhs.begin();
    auto rhs_it = rhs.begin();

    for (; lhs_it != lhs.end(); ++lhs_it, ++rhs_it) {
        const double difference = std::abs(*lhs_it - *rhs_it);
        sum += std::pow(difference, exponent);
    }
    return std::pow(sum, 1.0 / exponent);
}
} // namespace

double compute_distance(FeatureView lhs, FeatureView rhs, DistanceConfig config) {
    validate_inputs(lhs, rhs);
    validate_config(config);

    switch (config.metric) {
    case DistanceMetric::Euclidean: {
        return euclidean_distance(lhs, rhs);
    }
    case DistanceMetric::Manhattan: {
        return manhattan_distance(lhs, rhs);
    }
    case DistanceMetric::Chebyshev: {
        return chebyshev_distance(lhs, rhs);
    }
    case DistanceMetric::Canberra: {
        return canberra_distance(lhs, rhs);
    }
    case DistanceMetric::Minkowski: {
        const double exponent = validate_minkowski_config(config);
        return minkowski_distance(lhs, rhs, exponent);
    }
    }
    throw std::logic_error("Unknown distance metric");
}
} // namespace knn
