#pragma once

namespace JimmyGod::ML
{
	class GeneticAlgorithm
	{
	public:
		// Genome -> chromosomes -> genes
		struct Genome
		{
			std::vector<int> chromosomes; // [gene][gene][gene]
			float fitness = 0.0f;
		};

		using GeneratePopulation = std::function<void(std::vector<Genome>&, size_t)>;
		using ComputeFitnessFunc = std::function<float(const Genome&, float)>;
		using ComputeCrossOver = std::function<void(Genome&, const Genome&, const Genome&,float)>;
		using ComputeMutation = std::function<void(Genome&, float, int)>;
		
		void Initialize(int populationSize, int chromoLength,
			int maxGeneValue, float crossoverRate, float mutationRate, float bestFitness, GeneratePopulation generatePopulation,
			ComputeFitnessFunc computeFitness, ComputeCrossOver computeCrossover, ComputeMutation computeMutation);
		void Advance();

		const Genome& GetBestGenome() const { return mPopulation.front(); }
		int GetGeneration() const { return mGeneration; }
		
	private:
		void EvaluatePopulation();

		Genome Mate(const Genome& parent1, const Genome& parent2);

		GeneratePopulation mGeneratePopulation;
		ComputeFitnessFunc mComputeFitness;
		ComputeCrossOver mComputeCrossover;
		ComputeMutation mComputeMutation;

		std::vector<Genome> mPopulation;
		int mGeneration = 0;

		float mCrossoverRate = 0.0f;
		float mMutationRate = 0.0f;
		float mBestFitness = 0.0f;
		int mMaxGeneValue = 0;
	};
}