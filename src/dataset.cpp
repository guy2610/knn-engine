#include "knn/dataset.hpp"

#include <cmath>
#include <cstddef>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace knn {
Dataset::Dataset(std::vector<TrainingSample> samples) {
    if (samples.empty()) {
        throw std::invalid_argument("Dataset is empty");
    }
    if (samples.front().features.empty()) {
        throw std::invalid_argument("Dataset has no features");
    }

    dimension_ = samples.front().features.size();

    std::size_t sample_index = 0;

    for (const auto& sample : samples) {
        if (sample.features.size() != dimension_) {
            throw std::invalid_argument("Dataset with the sample " + std::to_string(sample_index) +
                                        " inconsistent dimensionality");
        }

        for (const double value : sample.features) {
            if (!std::isfinite(value)) {
                throw std::invalid_argument("Dataset with the sample " +
                                            std::to_string(sample_index) + " has value not finite");
            }
        }

        if (sample.label.empty()) {
            throw std::invalid_argument("Dataset with the sample " + std::to_string(sample_index) +
                                        " label is empty");
        }

        if (sample.label.find_first_of(",\r\n") != std::string::npos) {
            throw std::invalid_argument("Dataset with the sample " + std::to_string(sample_index) +
                                        " label is not valid");
        }

        ++sample_index;
    }

    samples_ = std::move(samples);
}

std::size_t Dataset::size() const noexcept {
    return samples_.size();
}

std::size_t Dataset::dimension() const noexcept {
    return dimension_;
}

const TrainingSample& Dataset::sample(std::size_t index) const {
    return samples_.at(index);
}

std::span<const TrainingSample> Dataset::samples() const noexcept {
    return samples_;
}
} // namespace knn
