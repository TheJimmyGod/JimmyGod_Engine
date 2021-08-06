#pragma once

#include "Component.h"
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

		void ChangeState(std::string stateName) { mStateMachine->ChangeState(stateName);}
		void Stop() 
		{ 
			mSpeed = 0.0f;
			ChangeState("Idle"); 
		}
		void Movement(const Vector3& pos);

		const JimmyGod::Math::Vector3& GetPosition() const;
		const float GetSpeed() const;

		float mMovementSpeed = 0.0f;
		float mMass = 0.0f;
		float mRadius = 0.0f;
		int mArea = 0;

		TransformComponent* GetTransformComponent();

		std::vector<Math::Vector3> mPath;
		/*Math::Vector3 mDestination = Math::Vector3::Zero;*/
	private:
		std::unique_ptr<JimmyGod::AI::StateMachine<AgentComponent>> mStateMachine;
	private:
		bool isDebugUI = false;
		bool isActive = true;
		float mSpeed = 0.0f;

		TransformComponent* mTransformComponent = nullptr;
		const ColliderComponent* mColliderComponent = nullptr;
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
		void Enter(AgentComponent& agent)
		{
			currentWaypoint = 0;
			maxWaypoint = agent.mPath.size();
		}
		void Update(AgentComponent& agent, float deltaTime)
		{
			if ((currentWaypoint == maxWaypoint) || (agent.mPath.size() == 0))
			{
				agent.Stop();
				return;
			}
			else
			{
				Destinination = Vector3(agent.mPath[currentWaypoint].x, agent.mPath[currentWaypoint].y, agent.mPath[currentWaypoint].z);
				if (Math::Distance(agent.GetPosition(), Destinination) > 0.1f)
				{
					Vector3 direction = Normalize(Destinination - agent.GetPosition());
					Vector3 nextPos = agent.GetPosition() + (direction * (agent.mMovementSpeed / agent.mMass) * deltaTime);
					agent.Movement(nextPos);
				}
				else{  currentWaypoint++; }
			}
		}
		void Exit(AgentComponent& agent) 
		{
			currentWaypoint = 0;
		}
	private:
		int maxWaypoint = 0;
		int currentWaypoint = 0;
		Math::Vector3 Destinination = Math::Vector3::Zero;
	};
}