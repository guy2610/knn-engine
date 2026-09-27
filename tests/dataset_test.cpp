#include "knn/dataset.hpp"
#include "knn/features.hpp"

#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
TEST(DatasetTest, ConstructsValidDataset) {
    const knn::TrainingSample sample1{.features = {3.0, 3.1, 2.0 / 3.0}, .label = "hi"};
    const knn::TrainingSample sample2{.features = {2.2, 3.0, 1.0}, .label = "bye"};
    const std::vector<knn::TrainingSample> samples{sample1, sample2};
    const knn::Dataset dataset(samples);

    EXPECT_EQ(dataset.size(), 2U);
    EXPECT_EQ(dataset.dimension(), 3U);

    EXPECT_EQ(dataset.sample(0).label, "hi");
    EXPECT_EQ(dataset.sample(1).label, "bye");

    const knn::FeatureVector expected_features{3.0, 3.1, 2.0 / 3.0};
    EXPECT_EQ(dataset.sample(0).features, expected_features);

    const knn::FeatureVector expected_features2{2.2, 3, 1};
    EXPECT_EQ(dataset.sample(1).features, expected_features2);
}

TEST(DatasetTest, RejectsEmptyDataset) {
    std::vector<knn::TrainingSample> const samples;
    EXPECT_THROW(knn::Dataset{samples}, std::invalid_argument);
}

TEST(DatasetTest, RejectsEmptyFeatureVector) {
    std::vector<knn::TrainingSample> const samples = {{.features = {}, .label = "class"}};
    EXPECT_THROW(knn::Dataset{samples}, std::invalid_argument);
}

TEST(DatasetTest, RejectsInconsistentDimensions) {
    const knn::TrainingSample sample1{.features = {3.0, 3.1, 2.0 / 3.0}, .label = "hi"};
    const knn::TrainingSample sample2{.features = {2.2, 1.0}, .label = "bye"};
    const std::vector<knn::TrainingSample> samples{sample1, sample2};

    EXPECT_THROW(knn::Dataset{samples}, std::invalid_argument);
}

TEST(DatasetTest, RejectsOutOfRangeSampleIndex) {
    const knn::TrainingSample example1{.features = {1.0, 2.2}, .label = "hi"};
    const std::vector<knn::TrainingSample> samples{example1, example1};
    const knn::Dataset dataset{samples};

    EXPECT_THROW(static_cast<void>(dataset.sample(2)), std::out_of_range);
}

class InvalidLabelTest : public ::testing::TestWithParam<std::string> {};

TEST_P(InvalidLabelTest, RejectsInvalidLabel) {
    const knn::TrainingSample sample{.features = {1.0, 2.0}, .label = GetParam()};
    const std::vector<knn::TrainingSample> samples{sample};

    EXPECT_THROW(knn::Dataset{samples}, std::invalid_argument);
}

INSTANTIATE_TEST_SUITE_P(InvalidLabels, InvalidLabelTest,
                         ::testing::Values(std::string{}, std::string{"bad,label"},
                                           std::string{"bad\nlabel"}, std::string{"bad\rlabel"}));

class NonFiniteFeatureTest : public ::testing::TestWithParam<double> {};

TEST_P(NonFiniteFeatureTest, RejectsNonFiniteFeature) {
    const knn::TrainingSample sample{.features = {1.0, GetParam(), 3.0}, .label = "class"};
    const std::vector<knn::TrainingSample> samples{sample};

    EXPECT_THROW(knn::Dataset{samples}, std::invalid_argument);
}

INSTANTIATE_TEST_SUITE_P(NonFiniteValues, NonFiniteFeatureTest,
                         ::testing::Values(std::numeric_limits<double>::quiet_NaN(),
                                           std::numeric_limits<double>::infinity(),
                                           -std::numeric_limits<double>::infinity()));

TEST(DatasetTest, ReturnsSamplesInDatasetOrder) {
    const knn::TrainingSample first{.features = {1.0, 2.0}, .label = "first"};
    const knn::TrainingSample second{.features = {3.0, 4.0}, .label = "second"};
    const knn::TrainingSample third{.features = {5.0, 6.0}, .label = "third"};

    const std::vector<knn::TrainingSample> samples{
        first,
        second,
        third,
    };

    const knn::Dataset dataset{samples};

    const auto view = dataset.samples();

    ASSERT_EQ(view.size(), 3U);

    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)

    EXPECT_EQ(view[0].label, "first");
    EXPECT_EQ(view[1].label, "second");
    EXPECT_EQ(view[2].label, "third");

    EXPECT_EQ(view[0].features, first.features);
    EXPECT_EQ(view[1].features, second.features);
    EXPECT_EQ(view[2].features, third.features);

    // NOLINTEND(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
}
} // namespace