#pragma once

#include "SteeringBehavior.h"
#include "Agent.h"

namespace JimmyGod::AI
{
	class SeekBehavior : public JimmyGod::AI::SteeringBehavior
	{
	public:
		JimmyGod::Math::Vector2 Calculate(Agent& agent) override;
		static JimmyGod::Math::Vector2 Seek(Agent& agent, JimmyGod::Math::Vector2 destination);
	};
}