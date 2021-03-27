#include "Precompiled.h"
#include "LinearRegression.h"

#include "Datasets.h"

using namespace JimmyGod::ML;

float LinearRegression::Fit(const Dataset& dataset)
{
	// Homework
	// b0 -= ???;
	// b1 -= ???;
	float y_pred = 0.0f;
	float error = 0.0f;
	const float dataSize = static_cast<float>(dataset.x0.size());

	float predB0 = 0.0f;
	float predB1 = 0.0f;

	float MSE = 0.0f;
	float AverageError = 0.0f;

	for (int i = 0; i < dataset.x0.size(); ++i)
	{
		// Prediction
		y_pred = Predict(dataset.x0[i]); // Compute the predict value
		// Gradient Descent
		// Taking the partial derivative of coefficients
		error = y_pred - dataset.y[i]; // Average error value

		predB0 += error;
		predB1 += error * dataset.x0[i];

		MSE += Math::Sqr(error);
	}
	predB0 /= dataSize;
	predB1 /= dataSize;

	b0 = b0 - learningRate * predB0;
	b1 = b1 - learningRate * predB1;

	AverageError = MSE / dataSize;

	return AverageError;
}

float LinearRegression::Predict(float x) const
{
	return b0 + (b1 * x);
}