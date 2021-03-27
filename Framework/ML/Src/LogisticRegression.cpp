#include "Precompiled.h"
#include "LogisitcRegression.h"
#include <ImGui/Inc/ImPlot.h>
#include "Datasets.h"

using namespace JimmyGod::ML;

float LogisticRegression::Fit(const Dataset& dataset)
{
	float db0 = 0.0f;
	float db1 = 0.0f; 
	float db2 = 0.0f; 
	float Cost = 0.0f; // TODO: fix to use log error functions

	for (size_t i = 0; i < dataset.x0.size(); i++)
	{
		float prediction = Predict(dataset.x0[i], dataset.x1[i]);
		float error = prediction - dataset.y[i];
		// Cost = (-1 / m)*(np.sum((Y_T*np.log(final_result)) + ((1 - Y_T)*(np.log(1 - final_result)))))
		db0 += error;
		db1 += error * dataset.x0[i];
		db2 += error * dataset.x1[i];

		Cost += error * error * error;
	}

	const float m = static_cast<float>(dataset.x0.size());
	b0 -= learningRate * db0 / m;
	b1 -= learningRate * db1 / m;
	b2 -= learningRate * db2 / m;

	return Cost / m;
}
float LogisticRegression::Predict(float x1, float x2) const
{
	float z = b0 + (b1 * x1) + (b2 * x2);
	return 1 / (1 + exp(-z));
}