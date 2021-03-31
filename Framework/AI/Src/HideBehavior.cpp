#include "Precompiled.h"
#include "HideBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 HideBehavior::Calculate(Agent & agent)
{
	if (agent.world.GetObstacles().size() == 0)
	{
		return JimmyGod::Math::Vector2();
	}

	float minDistance = std::numeric_limits<float>::infinity();
	JimmyGod::Math::Vector2 nearest = JimmyGod::Math::Vector2::Zero;
	for (auto& obs : agent.world.GetObstacles())
	{
		auto hidingSpot = GetHidingPosition(obs.center, obs.radius, agent.threat->Position);
		float distance = JimmyGod::Math::DistanceSqr(hidingSpot, agent.Position);

		if (distance < minDistance)
		{
			minDistance = distance;
			nearest = hidingSpot;
		}
	}

	if (minDistance == std::numeric_limits<float>::infinity())
	{
		JimmyGod::Math::Vector2 distance = agent.threat->Position - agent.Position;
		const float ahead = JimmyGod::Math::Magnitude(distance);
		float distanceAhead = ahead / agent.MaxSpeed;

		agent.Destination = agent.threat->Position + agent.threat->Velocity* distanceAhead;

		JimmyGod::Math::Vector2 desiredVel = JimmyGod::Math::Normalize(agent.Position - agent.Destination) * agent.MaxSpeed;
		return desiredVel - agent.Velocity;
	}
	agent.Destination = nearest;
	JimmyGod::Math::Vector2 toTarget = agent.Destination - agent.Position;
	float distance = JimmyGod::Math::Magnitude(toTarget);
	float decel = 1.0f;
	if (distance > 0.0f)
	{
		float decelTweaker = 0.3f;
		float speed = distance / decel * decelTweaker;
		speed = JimmyGod::Math::Min(speed, agent.MaxSpeed);
		JimmyGod::Math::Vector2 velocity = toTarget * speed / distance;
		return velocity - agent.Velocity;
	}
	else
	{
		return JimmyGod::Math::Vector2();
	}

}
