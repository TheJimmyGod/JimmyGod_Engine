#pragma once

namespace JimmyGod::ML
{
	struct Dataset
	{
		std::vector<float> x0;
		std::vector<float> x1;
		std::vector<float> y;
	};

	// TODO: Templatize the dataset feature dimenstion
	//template <size_t N>
	//struct Dataset
	//{
	//	std::vector < array<float, N>> X;
	//	std::vector<float> y;
	//};

	namespace Datasets
	{
		Dataset MakeLinear(size_t samples, float b0, float b1, float minX, float maxX, float noise);
		Dataset MakeLogistic(size_t samples, float b0, float b1, float b2, float minX, float maxX, float noise);
	}
}