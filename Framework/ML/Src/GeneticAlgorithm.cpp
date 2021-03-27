#include "Precompiled.h"
#include "GeneticAlgorithm.h"

using namespace JimmyGod::ML;
using namespace JimmyGod::Math;

void GeneticAlgorithm::Initialize(int populationSize, int chromoLength,
	int maxGeneValue, float crossoverRate, float mutationRate, float BestFitness, GeneratePopulation generatePopulation, ComputeFitnessFunc computeFitness, ComputeCrossOver computeCrossover, ComputeMutation computeMutation/*, GeneratePopulation generatePopulation*/)
{
	mGeneratePopulation = std::move(generatePopulation);
	mComputeFitness = std::move(computeFitness);
	mComputeCrossover = std::move(computeCrossover);
	mComputeMutation = std::move(computeMutation);

	// Reset
	mGeneration = 0;
	mMaxGeneValue = maxGeneValue;
	mCrossoverRate = crossoverRate;
	mMutationRate = mutationRate;
	mBestFitness = BestFitness;

	// Create initial population
	mPopulation.clear();
	//mPopulation.resize(populationSize);
	//for (auto& genome : mPopulation)
	//{
	//	genome.chromosomes.reserve(chromoLength);
	//	for (int i = 0; i < chromoLength; ++i)
	//		genome.chromosomes.push_back(RandomInt(0, maxGeneValue));
	//}

	mGeneratePopulation(mPopulation, populationSize);
	EvaluatePopulation();
}

void GeneticAlgorithm::Advance()
{
	++mGeneration;

	std::vector<Genome> newPopulation;
	newPopulation.reserve(mPopulation.size());

	// Perform Elitism: keep top 10%, mate the rest using top 50%
	const int eliteCount = ((int)mPopulation.size() * 10) / 100;
	for (int i = 0; i < eliteCount; ++i)
		newPopulation.push_back(mPopulation[i]);

	const int mateCount = (int)mPopulation.size() - eliteCount;
	const int cutoff = (int)mPopulation.size() / 2;
	for (int i = 0; i < mateCount; ++i)
	{
		const auto& parent1 = mPopulation[RandomInt(0, cutoff)];
		const auto& parent2 = mPopulation[RandomInt(0, cutoff)];
		newPopulation.push_back(Mate(parent1, parent2));
	}

	mPopulation = std::move(newPopulation);
	EvaluatePopulation();
}

void GeneticAlgorithm::EvaluatePopulation()
{
	for (auto& genome : mPopulation)
		genome.fitness = mComputeFitness(genome, mBestFitness);
	std::sort(mPopulation.begin(), mPopulation.end(), [](const auto& a, const auto& b)
	{
		return a.fitness > b.fitness;
	});
}

GeneticAlgorithm::Genome GeneticAlgorithm::Mate(const Genome & parent1, const Genome & parent2)
{
	Genome offspring;
	offspring.chromosomes.reserve(parent1.chromosomes.size());
	
	mComputeCrossover(offspring, parent1, parent2, mCrossoverRate);
	mComputeMutation(offspring, mMutationRate, mMaxGeneValue);

	return offspring;
}


// For homework,
// 1) follow geeksforgeeks example to test GA
// 2) allow crossover/mutate to be std::functions and use GA for pathfinding