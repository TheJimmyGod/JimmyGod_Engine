#pragma once
#include "SteeringBehavior.h"
#include "Agent.h"
#include "AIWorld.h"
#include "ArriveBehavior.h"
namespace JimmyGod::AI
{
	class AIWorld;

	class HideBehavior : public JimmyGod::AI::SteeringBehavior
	{
	public:
		JimmyGod::Math::Vector2 Calculate(Agent& agent) override;
		AIWorld* mAIWorld;
	private:
		JimmyGod::Math::Vector2 GetHidingPosition(const JimmyGod::Math::Vector2& pos, const float rad, const JimmyGod::Math::Vector2& posTarget)
		{
			const float distanceFromBoundary = 30.0f;
			const float DistAway = rad + distanceFromBoundary;

			JimmyGod::Math::Vector2 toOb = JimmyGod::Math::Normalize(pos - posTarget);

			return (toOb * DistAway) + pos;
		}
	};
}