#include <cmath>
#include <stdexcept>
#include <utility>

#include "knn/dataset.hpp"

namespace knn {
    Dataset::Dataset(std::vector<TrainingSample> samples) {
            if (samples.empty()) {
               throw std::invalid_argument("Dataset is empty");
            }
            if (samples.front().features.empty()) {
                throw std::invalid_argument("Dataset has no features");
            }

            dimension_ = samples.front().features.size();

            for (std::size_t i = 0; i < samples.size(); ++i) {
                if (samples[i].features.size() != dimension_) {
                    throw std::invalid_argument("Dataset with the sample " + std::to_string(i)  + " inconsistent dimensionality");
                }
                for (double value : samples[i].features) {
                    if (!std::isfinite(value)) {
                        throw std::invalid_argument("Dataset with the sample " + std::to_string(i)  + " has value not finite");
                    }
                }
                if (samples[i].label.empty()) {
                    throw std::invalid_argument("Dataset with the sample " + std::to_string(i)  + " label is empty");
                }
                if (samples[i].label.find_first_of(",\r\n") != std::string::npos) {
                    throw std::invalid_argument("Dataset with the sample " + std::to_string(i)  + " label is not valid");
                }
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
}
