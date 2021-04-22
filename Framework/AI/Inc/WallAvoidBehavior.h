#pragma once

#include "SteeringBehavior.h"
#include "Agent.h"
#include "AIWorld.h"

namespace JimmyGod::AI
{
	class WallAvoidBehvior : public JimmyGod::AI::SteeringBehavior
	{
	public:
		JimmyGod::Math::Vector2 Calculate(Agent& agent) override;
		AIWorld* mAIWorld;
	private:
		
	};
}