#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "GameManager.h"

using namespace JimmyGod;
using namespace JimmyGod::AI;

namespace JimmyCom
{
	class Attack : public AI::Strategy <AgentComponent>
	{
		float CalculateDesirability(AgentComponent& agent) const override
		{
			float desireability = 0.0f;
			float dist = 0.0f;
			float percent = 0.0f;
			if (!GameManager::Get())
				return 0.0f;

			for (int i = 0; i < GameManager::Get()->GetSoldierCount(); ++i)
			{
				auto& unit = GameManager::Get()->GetSoldier(i);
				if (unit->GetFlag() != Flag::Ally)
					continue;

				dist = Distance(agent.GetTransformComponent()->GetPosition(), unit->GetPosition());
				percent = Math::Clamp((dist / agent.GetViewRange()), 0.0f, 1.0f);
				if ((1-percent) * 100.0f > desireability)
					desireability = agent.GetViewRange() - Distance(agent.GetTransformComponent()->GetPosition(), unit->GetPosition());
			}
			for (int i = 0; i < GameManager::Get()->GetMutantCount(); ++i)
			{
				auto& unit = GameManager::Get()->GetMutant(i);
				if (unit->GetFlag() != Flag::Ally)
					continue;
				dist = Distance(agent.GetTransformComponent()->GetPosition(), unit->GetPosition());
				percent = Math::Clamp((dist / agent.GetViewRange()), 0.0f, 1.0f);
				if ((1 - percent) * 100.0f > desireability)
					desireability = agent.GetViewRange() - Distance(agent.GetTransformComponent()->GetPosition(), unit->GetPosition());
			}

			return desireability;
		}
		std::unique_ptr<Goal<AgentComponent>> CreateGoal() const override
		{
			std::unique_ptr<KillUnit> killUnit = std::make_unique<KillUnit>();
			return killUnit;
		}
	};

	class KillUnit : public AI::Goal<AgentComponent>
	{
	public:
		void Activate()
		{
			mStatus = Status::Active;
		}
		Status Process()
		{
			ActivateIfInactive();
			return mStatus;
		}
		void Terminate()
		{

		}
	private:

	};

	class AttackUnit : public AI::GoalComposite<AgentComponent>
	{
	public:
		void Activate()
		{
			mStatus = Status::Active;
		}
		Status Process()
		{
			ActivateIfInactive();
			mStatus = ProcessSubgoals();
			return mStatus;
		}
		void Terminate()
		{

		}
	private:

	};

	class MoveToUnit : public AI::GoalComposite<AgentComponent>
	{
	public:
		void Activate()
		{
			mStatus = Status::Active;
		}
		Status Process()
		{
			ActivateIfInactive();
			mStatus = ProcessSubgoals();
			return mStatus;
		}
		void Terminate()
		{

		}
	private:

	};

	class FindPath : public AI::Goal<AgentComponent>
	{
	public:
		void Activate()
		{
			mStatus = Status::Active;
			for (size_t i = 0; i < GameManager::Get()->GetSoldierCount(); i++)
			{
				auto& unit = GameManager::Get()->GetSoldier(i);
				if (unit->GetFlag() != Flag::Ally) continue;
				auto dist = Distance(mAgent.GetPosition(), unit->GetAgentComponent().GetPosition());
				if (dist > bestDist)
				{
					bestDist = dist;
					mAgent.SetDestinationCoord(unit->GetAgentComponent().GetCurrentCoord());
				}
			}
			GridManager::Get()->GetGird().FindPath(mAgent.GetCurrentCoord(), mAgent.GetDestinationCoord(), mAgent.mPath);

			while (mAgent.mPath.size() > 2)
			{
				if (GameManager::Get()->IsExist(GridManager::Get()->GetGird().GetGraph().GetNode(mAgent.mPath[mAgent.mPath.size() - 1])->coordinate))
					mAgent.mPath.pop_back();
				else
					break;
			}
		}
		Status Process()
		{
			ActivateIfInactive();
			return mStatus;
		}
		void Terminate()
		{

		}
	private:
		float bestDist = 0.0f;
	};

	class FollowPath : public AI::Goal<AgentComponent>
	{
	public:
		void Activate()
		{
			mStatus = Status::Active;
		}
		Status Process()
		{
			ActivateIfInactive();
			return mStatus;
		}
		void Terminate()
		{

		}
	private:

	};

	class AttacktoUnit : public AI::Goal<AgentComponent>
	{
	public:
		void Activate()
		{
			mStatus = Status::Active;
		}
		Status Process()
		{
			ActivateIfInactive();
			return mStatus;
		}
		void Terminate()
		{

		}
	private:

	};
}