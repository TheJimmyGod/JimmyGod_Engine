#pragma once

namespace JimmyGod::ML
{
	struct Dataset;

	class LogisticRegression
	{
	public:
		float Fit(const Dataset& dataset);
		float Predict(float x1, float x2) const;

		float b0 = 0.0f;
		float b1 = 0.0f;
		float b2 = 0.0f;
		float learningRate = 0.01f;
	};
}