#pragma once
#include "SteeringBehavior.h"
#include "Agent.h"

namespace JimmyGod::AI
{
	class WanderBehavior : public JimmyGod::AI::SteeringBehavior
	{
	public:
		JimmyGod::Math::Vector2 Calculate(Agent& agent) override;

	private:
		JimmyGod::Math::Vector2 targetPos = JimmyGod::Math::Vector2::Zero;
	};
}