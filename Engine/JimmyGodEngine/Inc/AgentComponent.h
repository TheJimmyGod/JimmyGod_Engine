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

		void ChangeState(std::string stateName);

		void Dead();
		void Stop();
		void Movement(const Vector3& pos, float deltaTime);

		TransformComponent* GetTransformComponent() { return mTransformComponent; }
		const TransformComponent* GetTransformComponent() const { return mTransformComponent; }

		ColliderComponent* GetColliderComponent() { return mColliderComponent; }
		const ColliderComponent* GetColliderComponent() const { return mColliderComponent; }

		const float GetSpeed() const;

		float mMovementSpeed = 0.0f;
		float mMass = 0.0f;
		int mArea = 0;
		std::vector<Math::Vector3> mPath;
	private:
		std::unique_ptr<JimmyGod::AI::StateMachine<AgentComponent>> mStateMachine = nullptr;
		TransformComponent* mTransformComponent = nullptr;
		ColliderComponent* mColliderComponent = nullptr;

	private:
		bool isDebugUI = false;
		bool isActive = true;
		float mSpeed = 0.0f;
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