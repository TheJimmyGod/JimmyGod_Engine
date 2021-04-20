#pragma once

#include "SteeringBehavior.h"
#include "Agent.h"


namespace JimmyGod::AI
{
	class ArriveBehavior : public JimmyGod::AI::SteeringBehavior
	{
	public:
		JimmyGod::Math::Vector2 Calculate(Agent& agent) override;
		static JimmyGod::Math::Vector2 Arrive(Agent& agent, JimmyGod::Math::Vector2 destination, float decel, float decelTweaker);
	};
}