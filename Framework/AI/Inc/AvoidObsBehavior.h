#pragma once
#include "SteeringBehavior.h"
#include "Agent.h"
#include "AIWorld.h"

namespace JimmyGod::AI
{
	class AIWorld;

	class AvoidObsBehavior : public JimmyGod::AI::SteeringBehavior
	{
	public:
		JimmyGod::Math::Vector2 Calculate(Agent& agent) override;
		AIWorld* mAIWorld;
	private:

		bool lineIntersectsCircle(JimmyGod::Math::Circle& obs, JimmyGod::Math::Vector2 ahead, JimmyGod::Math::Vector2 ahead2)
		{
			return JimmyGod::Math::Distance(obs.center, ahead) <= obs.radius || JimmyGod::Math::Distance(obs.center, ahead2) <= obs.radius;
		}

		JimmyGod::Math::Circle FindNearestObstacle(Agent& agent, JimmyGod::Math::Vector2 direction)
		{
			if (agent.world.GetObstacles().empty())
			{
				JimmyGod::Math::Circle temp;
				temp.radius = -1;
				temp.center = { -1, -1 };
				return temp;
			}

			JimmyGod::Math::Circle nearestCircle = agent.world.GetObstacles().front();
			float nearestDistance = JimmyGod::Math::MagnitudeSqr(nearestCircle.center - agent.Position);
			for (auto obstacle : agent.world.GetObstacles())
			{
				float distance = JimmyGod::Math::MagnitudeSqr(obstacle.center - agent.Position);
				if (distance < nearestDistance)
				{
					auto heading = obstacle.center - agent.Position;
					auto dot = JimmyGod::Math::Dot(heading, direction);
					if (dot > 0)
					{
						nearestCircle = obstacle;
						nearestDistance = distance;
					}
				}
			}
			return nearestCircle;
		}

	};
}