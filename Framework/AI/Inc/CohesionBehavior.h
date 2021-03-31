#pragma once
#include "SeekBehavior.h"
#include "SteeringBehavior.h"
#include "Agent.h"


namespace JimmyGod::AI
{
	class CohesionBehavior : public SeekBehavior
	{
	public:
		JimmyGod::Math::Vector2 Calculate(Agent& agent) override;
	};
}