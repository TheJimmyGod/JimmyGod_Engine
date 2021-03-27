#include "GameState.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <ImGui/Inc/imgui.h>
#include <ImGui/Inc/ImPlot.h>
#include <ImGui/Src/imgui_internal.h>
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;
using namespace JimmyGod::ML;

enum Direction
{
	UP,DOWN,LEFT,RIGHT
};

namespace
{
	

	std::vector<Vector2> kTargetCities;
	std::vector<int> kTarget1;
	std::vector<int> kTarget2;
	const std::string kTarget = "Genetic Algorithm is coooool!";
	int kVaildGeneValue = 126 - 32; // fromm character '~' to ' ' in ASCII table
	const int KVaildGeneValueDirection = 4;
}

void GameState::Initialize()
{
	auto graphicsSystem = GraphicsSystem::Get();
	graphicsSystem->SetClearColor(Colors::Black);

	mAppLog.AddLog("Generating data ...\n");


}

void GameState::Terminate()
{
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;

	if (inputSystem->IsKeyDown(KeyCode::ESCAPE))
	{
		JimmyGod::MainApp().Quit();
		return;
	}
	if (inputSystem->IsKeyPressed(KeyCode::SPACE))
	{
		mAppLog.AddLog("Bam!\n");
	}

	if (mInitialized == true)
	{
		//auto& best = mGA.GetBestGenome();

		if (!mFound)
		{
			mGA.Advance();

			// Print the next best genome
			//std::string bestStr;
			auto& newBest = mGA.GetBestGenome();
			mAppLog.AddLog("Generation %d  ", mGA.GetGeneration());
			mAppLog.AddLog("Fitness: %f\n", newBest.fitness);
			for (auto& gene : newBest.chromosomes)
				mAppLog.AddLog("%s ", std::to_string(gene).c_str());

			mAppLog.AddLog("\n");
			
		}
		if (mGA.GetBestGenome().chromosomes == kTarget1 || mGA.GetBestGenome().chromosomes == kTarget2)
			mFound = true;
	}

}

void GameState::DebugUI()
{

	ImGui::Begin("Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::InputInt("Max Cities", &kMaxCityCount);
	ImGui::InputFloat("Radius", &mRadius);
	ImGui::Separator();

	if (ImGui::Button("Push!"))
	{
		kTargetCities.clear();
		kTarget1.clear();
		kTarget2.clear();

		float angle = 0.0f;
		float constant = 2.0f * Constants::Pi / kMaxCityCount;
		
		for (int i = 0; i < kMaxCityCount; ++i)
		{
			angle += constant;
			kTargetCities.push_back(Vector2( sinf(angle) * mRadius, cosf(angle) * mRadius));
		}

		kTarget1.resize(kMaxCityCount);

		std::iota(kTarget1.begin(), kTarget1.end(), 0);

		//for (int i = 0; i < kMaxCityCount; ++i)
		//	kTarget1.push_back(i);

		kTarget2.push_back(0);
		for (int i = kMaxCityCount - 1; i >= 1; --i)
			kTarget2.push_back(i);

		for (int i = 0; i < kMaxCityCount - 1; ++i)
			mBestDistance += Distance(kTargetCities[i], kTargetCities[i + 1]);
		for (size_t i = 0; i < kTargetCities.size(); ++i)
		{
			mAppLog.AddLog(" %f,%f  ", kTargetCities[i].x, kTargetCities[i].y);
			mAppLog.AddLog("\n");
		}
		mAppLog.AddLog("\n Best Fitness = %f", mBestDistance);
		mAppLog.AddLog("\n");

		kVaildGeneValue = kMaxCityCount - 1;

		mDraw = true;
	}
	ImGui::Separator();
	if(ImGui::Button("Clear!"))
	{
		kTargetCities.clear();
		kTarget1.clear();
		kTarget2.clear();
		mFound = false;
		mInitialized = false;
		mBestDistance = 0.0f;
		mAppLog.Clear();
	}
	ImGui::Separator();
	if (ImGui::Button("Run!"))
	{
		if (kTargetCities.empty())
			return;
		mAppLog.AddLog("\n");

		mAppLog.AddLog("Initializing new population ... \n");

		auto generatePopulation = [target = kTarget1](auto& population, size_t populationSize)
		{
			population.resize(populationSize);
			for (auto& genome : population)
			{
				genome.chromosomes.reserve(target.size());
				std::vector<int> cities;
				for (int i = 1; i < target.size(); ++i)
					cities.push_back(target[i]);
				genome.chromosomes.push_back(0);
				for (int i = 0; i < target.size() - 1; ++i)
				{
					const size_t size = cities.size();
					int randomIndex = RandomInt(0, size - 1);
					genome.chromosomes.push_back(cities[randomIndex]);
					cities.erase(cities.begin() + randomIndex);
				}
			}
		};

		auto computeFitness = [](auto& genome, float bestFitness)
		{
			float fitness = 0.0f;
			
			// One point per matching character
			for (int i = 0; i < genome.chromosomes.size() - 1; ++i)
				fitness += Distance(kTargetCities[genome.chromosomes[i]], kTargetCities[genome.chromosomes[i + 1]]);

			return bestFitness - fitness; // 10000(Best_path) - Your part's length
		};

		auto computeCrossOver = [](auto& offspring, auto& parent1, auto& parent2, float rate)
		{
			for (size_t i = 0; i < parent1.chromosomes.size(); ++i)
				offspring.chromosomes.push_back(parent1.chromosomes[i]);


			if (RandomFloat() < rate)
			{
				const int swapTarget_Left = parent1.chromosomes[RandomInt(0, kVaildGeneValue)];
				const int swapTarget_Right = parent2.chromosomes[RandomInt(0, kVaildGeneValue)];

				size_t index1 = 0;
				size_t index2 = 0;
				for (size_t j = 0; j < offspring.chromosomes.size(); ++j)
				{
					if (offspring.chromosomes[j] == swapTarget_Left)
						offspring.chromosomes[j] = swapTarget_Right;
					else if (offspring.chromosomes[j] == swapTarget_Right)
						offspring.chromosomes[j] = swapTarget_Left;
				}

				//std::swap(offspring.chromosomes[index1], offspring.chromosomes[index2]);
			}

			//for (size_t i = 1; i < parent1.chromosomes.size(); ++i)
			//{
			//	if (RandomFloat() < rate)
			//	{
			//		const int swapTarget_Left = parent1.chromosomes[i];
			//		const int swapTarget_Right = RandomInt(1, kVaildGeneValue);

			//		size_t index1 = 0;
			//		size_t index2 = 0;
			//		for (size_t j = 0; j < offspring.chromosomes.size(); ++j)
			//		{
			//			if (offspring.chromosomes[j] == swapTarget_Left)
			//				offspring.chromosomes[j] = swapTarget_Right;
			//			else if (offspring.chromosomes[j] == swapTarget_Right)
			//				offspring.chromosomes[j] = swapTarget_Left;
			//		}

			//		std::swap(offspring.chromosomes[index1], offspring.chromosomes[index2]);
			//	}
			//}

			//for (size_t i = 1; i < parent2.chromosomes.size(); ++i)
			//{
			//	if (RandomFloat() < rate)
			//	{
			//		const int swapTarget_Left = parent2.chromosomes[i];
			//		const int swapTarget_Right = RandomInt(1, kVaildGeneValue);

			//		size_t index1 = 0;
			//		size_t index2 = 0;
			//		for (size_t j = 0; j < offspring.chromosomes.size(); ++j)
			//		{
			//			if (offspring.chromosomes[j] == swapTarget_Left)
			//				offspring.chromosomes[j] = swapTarget_Right;
			//			else if (offspring.chromosomes[j] == swapTarget_Right)
			//				offspring.chromosomes[j] = swapTarget_Left;
			//		}

			//		std::swap(offspring.chromosomes[index1], offspring.chromosomes[index2]);
			//	}
			//}
		};

		auto computeMutation = [](auto& offspring, float rate, int maxValueGene)
		{
			if (RandomFloat() < rate)
			{
				int randomIndex = RandomInt(0, (int)(maxValueGene));
				int city = offspring.chromosomes[randomIndex];
				offspring.chromosomes.erase(offspring.chromosomes.begin() + randomIndex);
				offspring.chromosomes.push_back(city);
			}

		};

		mGA.Initialize(100, kMaxCityCount, kMaxCityCount - 1, 0.45f, 0.3f, mBestDistance, generatePopulation, computeFitness, computeCrossOver, computeMutation);

		mInitialized = true;
	}


	ImGui::End();
	ImGui::SetNextWindowSize({ 800.0f,600.0f });
	ImGui::Begin("Two", nullptr, ImGuiWindowFlags_NoResize);

	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	ImVec2 winPos = ImGui::GetWindowPos();
	ImVec2 offset = { 400.0f, 300.0f };

	if (mDraw)
	{
		
		for (size_t i = 0; i < kTargetCities.size(); ++i)
		{
			ImVec2 vec;
			vec.x = kTargetCities[i].x;
			vec.y = kTargetCities[i].y;
			drawList->AddCircle(winPos + (vec * 5.0f) + offset, mRadius, 0xff0000ff);

			std::string str = std::to_string(i);

			drawList->AddText(winPos + (vec * 5.0f) + offset, 0xff0000ff, str.c_str());
		}

	}

	if (mFound)
	{
		auto mBestDistance = mGA.GetBestGenome();
		ImVec2 first;
		first.x = kTargetCities[mBestDistance.chromosomes[0]].x;
		first.y = kTargetCities[mBestDistance.chromosomes[0]].y;

		ImVec2 last;
		last.x = kTargetCities[mBestDistance.chromosomes[kMaxCityCount - 1]].x;
		last.y = kTargetCities[mBestDistance.chromosomes[kMaxCityCount - 1]].y;
		drawList->AddCircle(winPos + (first*5.0f) + offset, mRadius + 5.0f, 0xFFFFFF00);
		drawList->AddCircle(winPos + (last *5.0f) + offset, mRadius + 5.0f, 0xff00ff00);
		for (size_t i = 0; i < kTargetCities.size() - 1; ++i)
		{
			ImVec2 vec;
			vec.x = kTargetCities[mBestDistance.chromosomes[i]].x;
			vec.y = kTargetCities[mBestDistance.chromosomes[i]].y;

			ImVec2 vec2;
			vec2.x = kTargetCities[mBestDistance.chromosomes[i+1]].x;
			vec2.y = kTargetCities[mBestDistance.chromosomes[i+1]].y;
			drawList->AddLine(winPos + (vec * 5.0f) + offset, winPos + (vec2 * 5.0f) + offset, 0xff0000ff);
		}
	}

	ImGui::End();

	mAppLog.Draw("Console");
}

