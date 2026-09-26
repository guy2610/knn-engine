#pragma once

#include <cstddef>
#include <span>
#include <string>
#include <vector>

#include "knn/features.hpp"


namespace knn {
    struct TrainingSample {
        FeatureVector features;
        std::string label;
    };

    class Dataset {
    public:
        explicit Dataset(std::vector<TrainingSample> samples);

        [[nodiscard]] std::size_t size() const noexcept;

        [[nodiscard]] std::size_t dimension() const noexcept;

        [[nodiscard]] const TrainingSample& sample(std::size_t index) const;

        [[nodiscard]] std::span<const TrainingSample> samples() const noexcept;

    private:
        std::vector<TrainingSample> samples_;
        std::size_t dimension_{0};
    };
}
