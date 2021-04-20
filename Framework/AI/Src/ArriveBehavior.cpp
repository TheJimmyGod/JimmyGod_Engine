#include"Precompiled.h"
#include"ArriveBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 ArriveBehavior::Calculate(Agent & agent)
{
	if (IsActive())
	{
		JimmyGod::Math::Vector2 toTarget = agent.Destination - agent.Position;
		float distance = JimmyGod::Math::Magnitude(toTarget);
		float decel = 3.0f;

		if (distance > 100.0f)
		{
			float decelTweaker = 0.3f;
			float speed = distance / decel * decelTweaker;
			speed = JimmyGod::Math::Min(speed, agent.MaxSpeed);
			JimmyGod::Math::Vector2 velocity = toTarget * speed / distance;
			return velocity - agent.Velocity;
		}
		else
		{
			JimmyGod::Math::Vector2 velocity = toTarget * agent.MaxSpeed / distance;
			return velocity - agent.Velocity;
		}
	}

	return JimmyGod::Math::Vector2{ 0,0 };
}
