#include "knn/distance.hpp"
#include "knn/features.hpp"

#include <gtest/gtest.h>
#include <limits>
#include <optional>
#include <stdexcept>
#include <vector>

namespace {
TEST(DistanceTest, ComputesEuclideanDistance) {
    const std::vector<double> first{3.0, 3.1, 0.0};
    const knn::FeatureView lhs{first};
    const std::vector<double> second{6.0, 7.1, 0.0};
    const knn::FeatureView rhs{second};

    EXPECT_NEAR(knn::compute_distance(lhs, rhs,
                                      knn::DistanceConfig{.metric = knn::DistanceMetric::Euclidean,
                                                          .minkowski_p = std::nullopt}),
                5.0, 1e-12);
}
TEST(DistanceTest, RejectsMismatchedDimensions) {
    const std::vector<double> first{3.0, 3.1, 0.0};
    const knn::FeatureView lhs{first};
    const std::vector<double> second{6.0, 7.1};
    const knn::FeatureView rhs{second};

    EXPECT_THROW(static_cast<void>(knn::compute_distance(
                     lhs, rhs,
                     knn::DistanceConfig{.metric = knn::DistanceMetric::Euclidean,
                                         .minkowski_p = std::nullopt})),
                 std::invalid_argument);
}

TEST(DistanceTest, RejectsEmptyFeatureVectors) {
    const std::vector<double> first{};
    const knn::FeatureView lhs{first};
    const std::vector<double> second{};
    const knn::FeatureView rhs{second};

    EXPECT_THROW(static_cast<void>(knn::compute_distance(
                     lhs, rhs,
                     knn::DistanceConfig{.metric = knn::DistanceMetric::Euclidean,
                                         .minkowski_p = std::nullopt})),
                 std::invalid_argument);
}

TEST(DistanceTest, RejectsMinkowskiWithoutP) {
    const std::vector<double> first{1.0, 2.0};
    const std::vector<double> second{3.0, 4.0};

    const knn::FeatureView lhs{first};
    const knn::FeatureView rhs{second};

    const knn::DistanceConfig config{
        .metric = knn::DistanceMetric::Minkowski,
        .minkowski_p = std::nullopt,
    };

    EXPECT_THROW(static_cast<void>(knn::compute_distance(lhs, rhs, config)), std::invalid_argument);
}

TEST(DistanceTest, RejectsMinkowskiPBelowOne) {
    const std::vector<double> first{1.0, 2.0};
    const std::vector<double> second{3.0, 4.0};

    const knn::FeatureView lhs{first};
    const knn::FeatureView rhs{second};

    const knn::DistanceConfig config{
        .metric = knn::DistanceMetric::Minkowski,
        .minkowski_p = 0.5,
    };

    EXPECT_THROW(static_cast<void>(knn::compute_distance(lhs, rhs, config)), std::invalid_argument);
}

class InvalidMinkowskiPTest : public ::testing::TestWithParam<double> {};

TEST_P(InvalidMinkowskiPTest, RejectsNonFiniteP) {
    const std::vector<double> first{1.0, 2.0};
    const std::vector<double> second{3.0, 4.0};

    const knn::FeatureView lhs{first};
    const knn::FeatureView rhs{second};

    const knn::DistanceConfig config{
        .metric = knn::DistanceMetric::Minkowski,
        .minkowski_p = GetParam(),
    };

    EXPECT_THROW(static_cast<void>(knn::compute_distance(lhs, rhs, config)), std::invalid_argument);
}

INSTANTIATE_TEST_SUITE_P(NonFiniteMinkowskiP, InvalidMinkowskiPTest,
                         ::testing::Values(std::numeric_limits<double>::quiet_NaN(),
                                           std::numeric_limits<double>::infinity(),
                                           -std::numeric_limits<double>::infinity()));

TEST(DistanceTest, RejectsMinkowskiPForOtherMetric) {
    const std::vector<double> first{1.0, 2.0};
    const std::vector<double> second{3.0, 4.0};

    const knn::FeatureView lhs{first};
    const knn::FeatureView rhs{second};

    const knn::DistanceConfig config{
        .metric = knn::DistanceMetric::Euclidean,
        .minkowski_p = 2.0,
    };

    EXPECT_THROW(static_cast<void>(knn::compute_distance(lhs, rhs, config)), std::invalid_argument);
}

TEST(DistanceTest, ComputesManhattanDistance) {
    const std::vector<double> first{3.0, 3.1, 0.0};
    const knn::FeatureView lhs{first};
    const std::vector<double> second{6.0, 7.1, 0.0};
    const knn::FeatureView rhs{second};

    EXPECT_NEAR(knn::compute_distance(lhs, rhs,
                                      knn::DistanceConfig{.metric = knn::DistanceMetric::Manhattan,
                                                          .minkowski_p = std::nullopt}),
                7.0, 1e-12);
}

TEST(DistanceTest, ComputesChebyshevDistance) {
    const std::vector<double> first{3.0, 3.1, 0.0};
    const knn::FeatureView lhs{first};
    const std::vector<double> second{6.0, 7.1, 0.0};
    const knn::FeatureView rhs{second};

    EXPECT_NEAR(knn::compute_distance(lhs, rhs,
                                      knn::DistanceConfig{.metric = knn::DistanceMetric::Chebyshev,
                                                          .minkowski_p = std::nullopt}),
                4.0, 1e-12);
}

TEST(DistanceTest, ComputesCanberraDistance) {
    const std::vector<double> first{2.0, 3.0};
    const knn::FeatureView lhs{first};
    const std::vector<double> second{3.0, 2.0};
    const knn::FeatureView rhs{second};

    EXPECT_NEAR(knn::compute_distance(lhs, rhs,
                                      knn::DistanceConfig{.metric = knn::DistanceMetric::Canberra,
                                                          .minkowski_p = std::nullopt}),
                0.4, 1e-12);
}
TEST(DistanceTest, HandlesCanberraZeroZeroCoordinate) {
    const std::vector<double> first{0.0, 1.0};
    const knn::FeatureView lhs{first};
    const std::vector<double> second{0.0, 3.0};
    const knn::FeatureView rhs{second};

    EXPECT_NEAR(knn::compute_distance(lhs, rhs,
                                      knn::DistanceConfig{.metric = knn::DistanceMetric::Canberra,
                                                          .minkowski_p = std::nullopt}),
                0.5, 1e-12);
}

TEST(DistanceTest, ComputesMinkowskiDistance) {
    const std::vector<double> first{1.0, 2.0, 1.0};
    const knn::FeatureView lhs{first};
    const std::vector<double> second{1.0, 2.0, 5.0};
    const knn::FeatureView rhs{second};

    EXPECT_NEAR(knn::compute_distance(lhs, rhs,
                                      knn::DistanceConfig{.metric = knn::DistanceMetric::Minkowski,
                                                          .minkowski_p = 3}),
                4.0, 1e-12);
}
TEST(DistanceTest, MinkowskiWithPOneMatchesManhattan) {
    const std::vector<double> first{0.0, 1.0};
    const knn::FeatureView lhs{first};
    const std::vector<double> second{0.0, 3.0};
    const knn::FeatureView rhs{second};

    const double manhattan = knn::compute_distance(
        lhs, rhs,
        knn::DistanceConfig{.metric = knn::DistanceMetric::Manhattan, .minkowski_p = std::nullopt});
    const double minkowski = knn::compute_distance(
        lhs, rhs, knn::DistanceConfig{.metric = knn::DistanceMetric::Minkowski, .minkowski_p = 1});
    EXPECT_NEAR(manhattan, minkowski, 1e-12);
}
TEST(DistanceTest, MinkowskiWithPTwoMatchesEuclidean) {
    const std::vector<double> first{0.0, 1.0};
    const knn::FeatureView lhs{first};
    const std::vector<double> second{0.0, 3.0};
    const knn::FeatureView rhs{second};

    const double euclidean = knn::compute_distance(
        lhs, rhs,
        knn::DistanceConfig{.metric = knn::DistanceMetric::Euclidean, .minkowski_p = std::nullopt});
    const double minkowski = knn::compute_distance(
        lhs, rhs, knn::DistanceConfig{.metric = knn::DistanceMetric::Minkowski, .minkowski_p = 2});

    EXPECT_NEAR(euclidean, minkowski, 1e-12);
}
} // namespace
