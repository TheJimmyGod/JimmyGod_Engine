#pragma once

#include "Goal.h"

namespace JimmyGod::AI
{
	template <class AgentType>
	class Strategy
	{
	public:
		virtual ~Strategy() = default;

		virtual float CalculateDesirability() const = 0;
		virtual std::unique_ptr<Goal<AgentType>> CreateGoal() const = 0;
	};
}