#pragma once
#include <memory>
#include <stdexcept>
#include <algorithm>

#include "multi_matrix.hpp"
#include "multi_matrix_factory.hpp"
#include "Layer/layers.hpp"
#include "Layer/filter.hpp"

namespace Convolutional {

	template <typename Classification>
	class NeuralNetwork {
	public:
		explicit NeuralNetwork(Layer::Layers layers) : layers{ std::move(layers) } {	}
		NeuralNetwork(const NeuralNetwork&) = default;
		NeuralNetwork(NeuralNetwork&&) = default;

		auto ClassifyMultiMatrix(const InputData::IInputData& input) {
			return ClassifyMultiMatrix(MultiMatrixFactory::GetMultiMatrix(input));
		}

		auto ClassifyMultiMatrix(const MultiMatrix& multiMatrix) {
			auto outputs = GetOutputsUsingMatrix(multiMatrix);
			auto maxOutput = std::max_element(outputs.begin(), outputs.end());
			auto outputIndex = std::distance(outputs.begin(), maxOutput);
			return static_cast<Classification>(outputIndex);
		}

		auto GetOutputsUsingMatrix(const MultiMatrix& multiMatrix) {
			auto processedMultiMatrix{ multiMatrix };
			for (const auto& layer : layers) {
				processedMultiMatrix = layer->ProcessMultiMatrix(processedMultiMatrix);
			}
			if (processedMultiMatrix.GetElementCount() != 1)
				throw std::logic_error("The last layer did not reduce the MultiMatrix to one element per dimension");

			std::vector<Matrix::element_t> outputs;
			outputs.reserve(processedMultiMatrix.GetDimensionCount());
			for (const auto& matrix : processedMultiMatrix) {
				outputs.push_back(matrix.ElementAt({ 0, 0 }));
			}
			return outputs;
		}

		Layer::Layers layers;
	};

}
