#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace JimmyGod
{
	class AgentMesh : public AI::Agent
	{
	public:
		enum DecisionState
		{
			Attack
		};

		AgentMesh(AI::AIWorld& aiWorld, uint32_t num) : AI::Agent(aiWorld, num) 
		{
			Initialize();
		}

		void Terminate();
		void Update(float deltaTime);

		void AddStrategy(DecisionState state);

		bool GetInitialized() const { return isInitialized; }

	private:
		void Initialize();
		std::unique_ptr<JimmyGod::AI::SteeringModule> mSteeringModule = nullptr;
		std::unique_ptr<JimmyGod::AI::DecisionModule<AgentMesh>> mDecisionModule = nullptr;
		bool isInitialized = false;
	};
}