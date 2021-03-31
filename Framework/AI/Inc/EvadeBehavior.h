#pragma once

#include "SteeringBehavior.h"
#include "Agent.h"

namespace JimmyGod::AI
{
	class EvadeBehavior : public JimmyGod::AI::SteeringBehavior
	{
	public:
		JimmyGod::Math::Vector2 Calculate(Agent& agent) override;
	};
}