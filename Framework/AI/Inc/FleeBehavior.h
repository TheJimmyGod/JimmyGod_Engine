#pragma once

#include "SteeringBehavior.h"
#include "Agent.h"

namespace JimmyGod::AI
{
	class FleeBehavior : public JimmyGod::AI::SteeringBehavior
	{
	public:
		JimmyGod::Math::Vector2 Calculate(Agent& agent) override;
		float panicDistance = 100.0f;
	};
}