#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace JimmyGod
{
	class TransformComponent;
	class ColliderComponent;

	class AIWorld;

	class AgentComponent final : public Component
	{
	public:
		META_CLASS_DECLARE

		void EnableDebug() { if (!isDebugUI) isDebugUI = true; else isDebugUI = false; }
		
		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;

		void Initialize_AgentMesh(AI::AIWorld& aiWorld, uint32_t num);
		void AddStrategy(DecisionState state);
		void ChangeState(std::string stateName);

		void ChangeState_AgentMesh(DecisionState state)
		{
			mCurrentThink = state;
		}

		void Dead();
		void Stop();
		void Movement(const Vector3& pos, float deltaTime);

		void SetCurrentCoord(const AI::Coord& coord) { mCurrCoord = coord; }
		const JimmyGod::AI::Coord& GetCurrentCoord() const { return mCurrCoord; }

		void SetDestinationCoord(const AI::Coord& coord) { mDestCoord = coord; }
		const JimmyGod::AI::Coord& GetDestinationCoord() const { return mDestCoord; }
		void SetDestination(const Vector3& pos) { mDestination = pos; }
		const JimmyGod::Math::Vector3& GetDestination() const { return mDestination; }
		const JimmyGod::Math::Vector3& GetPosition() const { return GetTransformComponent()->pos; }

		void SetViewRange(float range) { mViewRange = range; }
		const float GetViewRange() const { return mViewRange; }

		TransformComponent* GetTransformComponent() { return mTransformComponent; }
		const TransformComponent* GetTransformComponent() const { return mTransformComponent; }

		ColliderComponent* GetColliderComponent() { return mColliderComponent; }
		const ColliderComponent* GetColliderComponent() const { return mColliderComponent; }

		const float GetSpeed() const;

		AI::Agent* mAgentMesh = nullptr;

		float mMovementSpeed = 0.0f;
		float mMass = 0.0f;
		int mArea = 0;
		std::vector<Math::Vector3> mPath;

	private:
		std::unique_ptr<JimmyGod::AI::StateMachine<AgentComponent>> mStateMachine = nullptr;

		std::unique_ptr<JimmyGod::AI::SteeringModule> mSteeringModule = nullptr;
		std::unique_ptr<JimmyGod::AI::DecisionModule<AgentComponent>> mDecisionModule;

		TransformComponent* mTransformComponent = nullptr;
		ColliderComponent* mColliderComponent = nullptr;

	private:
		bool isDebugUI = false;
		bool isActive = true;
		bool isInitialized = false;
		DecisionState mCurrentThink = DecisionState::None;
		float mViewRange = 100.0f;
		float mSpeed = 0.0f;

		AI::Coord mDestCoord = { 0,0 };
		Math::Vector3 mDestination = Math::Vector3::Zero;
		AI::Coord mCurrCoord = { 0,0 };
	};

	class Idle : public JimmyGod::AI::State<AgentComponent>
	{
	public:
		void Enter(AgentComponent& agent) {}
		void Update(AgentComponent& agent, float deltaTime) {}
		void Exit(AgentComponent& agent) {}
	};

	class Move : public JimmyGod::AI::State<AgentComponent>
	{
	public:
		void Enter(AgentComponent& agent) override
		{
			currentWaypoint = 0;
			maxWaypoint = agent.mPath.size();
		}
		void Update(AgentComponent& agent, float deltaTime) override
		{
			if ((currentWaypoint == maxWaypoint) || (agent.mPath.size() == 0))
			{
				agent.Stop();
				return;
			}
			else
			{
				Destinination = Vector3(agent.mPath[currentWaypoint].x, agent.mPath[currentWaypoint].y, agent.mPath[currentWaypoint].z);
				if (Math::Distance(agent.GetTransformComponent()->GetPosition(), Destinination) > 0.1f)
				{
					Vector3 direction = Normalize(Destinination - agent.GetTransformComponent()->GetPosition());
					Vector3 nextPos = agent.GetTransformComponent()->GetPosition() + (direction * (agent.mMovementSpeed / agent.mMass) * deltaTime);
					agent.Movement(nextPos, deltaTime);
				}
				else{  currentWaypoint++; }
			}
		}
		void Exit(AgentComponent& agent) override
		{
			currentWaypoint = 0;
		}
	private:
		int maxWaypoint = 0;
		int currentWaypoint = 0;
		Math::Vector3 Destinination = Math::Vector3::Zero;
	};
}