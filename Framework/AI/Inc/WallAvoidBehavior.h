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

		bool LineIntersectsWall(Agent & agent, JimmyGod::Math::Vector2 sight1, 
			JimmyGod::Math::Vector2 sight2, 
			JimmyGod::Math::LineSegment wall)
		{
			auto point = GetDistanceWall(agent.Position, wall);


			return JimmyGod::Math::Magnitude(point - sight1) <= agent.Radius + 100.0f ||
				JimmyGod::Math::Magnitude(point - sight2) <= agent.Radius + 100.0f;
		}

		JimmyGod::Math::Vector2 GetDistanceWall(JimmyGod::Math::Vector2 pos, JimmyGod::Math::LineSegment wall) const
		{
			JimmyGod::Math::Vector2 a = pos;
			JimmyGod::Math::Vector2 b = wall.from;
			JimmyGod::Math::Vector2 c = wall.to;
			float wallLength = JimmyGod::Math::Sqrt(JimmyGod::Math::Sqr(wall.to.x - wall.from.x) + 
				JimmyGod::Math::Sqr(wall.to.y - wall.from.y));

			float area = JimmyGod::Math::Abs((a.x*(b.y - c.y) + b.x*(a.y - c.y) + c.x*(a.y - b.y)) * 0.5f);
			float h = ((area * 2.0f) / wallLength);
			float distance = JimmyGod::Math::Sqrt(JimmyGod::Math::Sqr(h) + 
				(JimmyGod::Math::Sqr(wall.from.x - pos.x) + JimmyGod::Math::Sqr(wall.from.y - pos.y)));

			JimmyGod::Math::Vector2 point, direction;

			direction = { wall.to.x - wall.from.x, wall.to.y - wall.from.y };
			direction = JimmyGod::Math::Normalize(direction);
			direction *= distance;
			point = wall.from + direction;

			return point * 0.5f;
		}

		float GetDistance(JimmyGod::Math::Vector2 pos, JimmyGod::Math::LineSegment wall) const
		{
			JimmyGod::Math::Vector2 a = pos;
			JimmyGod::Math::Vector2 b = wall.from;
			JimmyGod::Math::Vector2 c = wall.to;
			float wallLength = JimmyGod::Math::Sqrt(JimmyGod::Math::Sqr(wall.to.x - wall.from.x) + 
				JimmyGod::Math::Sqr(wall.to.y - wall.from.y));

			float area = JimmyGod::Math::Abs((a.x*(b.y - c.y) + b.x*(a.y - c.y) + c.x*(a.y - b.y)) * 0.5f);
			float h = ((area * 2) / wallLength);
			float distance = JimmyGod::Math::Sqrt(JimmyGod::Math::Sqr(h) + 
				(JimmyGod::Math::Sqr(wall.from.x - pos.x) + 
				JimmyGod::Math::Sqr(wall.from.y - pos.y)));

			return distance;
		}
	};
}