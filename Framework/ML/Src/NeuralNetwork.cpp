#include "Precompiled.h"
#include "NeuralNetwork.h"

using namespace JimmyGod::ML;
using namespace JimmyGod::Math;

namespace
{
	float Activation(float x)
	{
		return tanh(x);
	}
	float ActivationDerivative(float x)
	{
		return 1 - Sqr(x);
	}


	// This works better for XOR
	double Tanh(double x)
	{
		return tanh(x);
	}

	double TanhDerivative(double x)
	{
		return 1 - (x * x);
	}


	// This works better for digits
	double Sigmoid(double x)
	{
		return 1 / (1 + exp(-x));
	}

	double SigmoidDerivative(double x)
	{
		double out = Sigmoid(x);
		return out * (1 - out);
	}

}

Neuron::Neuron(size_t numOutputs, size_t myIndex, bool isBias)
	:mMyIndex(myIndex)
{
	if (isBias)
	{
		mOutputWeights.resize(numOutputs, { 1.0f });
	}
	else
	{
		for (size_t i = 0; i < numOutputs; ++i)
			mOutputWeights.push_back({ RandomFloat() });
	}

}

void Neuron::FeedForward(const Layer& previousLayer)
{
	float sum = 0.0f;
	for (size_t neuron = 0; neuron < previousLayer.size(); ++neuron)
	{
		auto& n =  previousLayer[neuron];
		sum += n.GetOutputValue() * n.mOutputWeights[mMyIndex].weight;
	}
	mOutputValue = Activation(sum);
}

void Neuron::CalculateOutputGradients(float targetValue)
{
	float error = targetValue - mOutputValue;
	mGradient = error * ActivationDerivative(mOutputValue);
}

void Neuron::CalculateHiddenGradients(const Layer& nextLayer)
{
	float sumDOW = 0.0f;
	for (size_t neuron = 0; neuron + 1 < nextLayer.size(); ++neuron)
		sumDOW += mOutputWeights[neuron].weight * nextLayer[neuron].mGradient;
	mGradient = sumDOW * ActivationDerivative(mOutputValue);
}

void Neuron::UpdateInputWeights(Layer& previousLayer)
{
	const float learningRate = 0.15f;

	for (size_t neuron = 0; neuron < previousLayer.size(); ++neuron)
	{
		auto& n = previousLayer[neuron];
		float deltaWeight = learningRate * n.GetOutputValue() * mGradient;
		n.mOutputWeights[mMyIndex].weight += deltaWeight;
	}
}

NeuralNetwork::NeuralNetwork(const std::vector<size_t>& topology)
{
	const size_t numLayers = topology.size();
	ASSERT(numLayers >= 2, "NeuralNetwork -- Invalid topology. Must have at least 2 layers");

	mLayers.resize(numLayers);
	for (size_t layer = 0; layer < numLayers; ++layer)
	{
		// Get number of outputs by looking at the neuron count for the next layer.
		// However, the last layer (i.e. the output) has no next layer.
		const size_t numOutputs = (layer + 1 == numLayers) ? 0 : topology[layer + 1];
		const size_t numNeurons = topology[layer];

		// For each layer, we want 1 extra neuron as the bias neuron (hence <= instead of <)
		for (size_t neuron = 0; neuron <= numNeurons; ++neuron)
			mLayers[layer].emplace_back(numOutputs, neuron, neuron == numNeurons);

	}

}
// Bias = it is for folding in the threshold

void NeuralNetwork::FeedForward(const std::vector<float>& inputValues)
{
	// "-1 is for removing bias"
	ASSERT(inputValues.size() == mLayers.front().size() - 1, "NeuralNetwork -- Invalid input count.");

	// Assign the input values into the input layer neurons
	for (size_t i = 0; i < inputValues.size(); ++i)
		mLayers.front()[i].SetOutputValue(inputValues[i]);

	// Forward propagete
	for (size_t layer = 0; layer + 1 < mLayers.size(); ++layer)
	{
		auto& currentLayer = mLayers[layer];
		auto& nextLayer = mLayers[layer + 1];

		for (size_t neuron = 0; neuron + 1 < nextLayer.size(); ++neuron)
			nextLayer[neuron].FeedForward(currentLayer);
	}
}

void NeuralNetwork::BackPropagate(const std::vector<float>& targetValues)
{
	ASSERT(targetValues.size() == mLayers.back().size() - 1, "NeuralNetwork -- Invalid input count.");

	Layer& outputLayer = mLayers.back();

	// Calculate output layer gradient
	for (size_t neuron = 0; neuron + 1 < outputLayer.size(); ++neuron)
		outputLayer[neuron].CalculateOutputGradients(targetValues[neuron]);

	// Calculate gradients on hidden layers
	for (size_t layer = mLayers.size() - 2; layer > 0; --layer)
	{
		Layer& hiddenLayer = mLayers[layer];
		Layer& nextLayer = mLayers[layer + 1];

		for (size_t neuron = 0; neuron < hiddenLayer.size(); neuron++)
			hiddenLayer[neuron].CalculateHiddenGradients(nextLayer);
	}

	// Update connection weights
	for (size_t layer = mLayers.size() - 1; layer > 0; --layer)
	{
		Layer& currentLayer = mLayers[layer];
		Layer& previousLayer = mLayers[layer - 1];
		for (size_t neuron = 0; neuron + 1 < currentLayer.size(); ++neuron)
			currentLayer[neuron].UpdateInputWeights(previousLayer);
	}
}

std::vector<float> NeuralNetwork::GetResults() const
{
	std::vector<float> results;
	const Layer& outputLayer = mLayers.back();
	for (size_t neuron = 0; neuron + 1 < outputLayer.size(); ++neuron)
		results.push_back(outputLayer[neuron].GetOutputValue());
	return results;
}