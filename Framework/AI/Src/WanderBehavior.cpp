#include "Precompiled.h"
#include "WanderBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 WanderBehavior::Calculate(Agent & agent)
{
	if (IsActive())
	{
		float wanderDistance = 100.0f;
		const float wanderJitter = 5.0f;
		JimmyGod::Math::Vector2 circle = (agent.Position + (agent.Heading * wanderDistance));

		JimmyGod::Math::Vector2 random; // WanderJitter
		random.x = JimmyGod::Math::RandomFloat(-1.0f, 1.0f) * wanderJitter;
		random.y = JimmyGod::Math::RandomFloat(-1.0f, 1.0f) * wanderJitter;

		targetPos += random;
		targetPos = JimmyGod::Math::Normalize(targetPos);
		targetPos *= agent.Radius;

		agent.Destination = targetPos + circle;

		JimmyGod::Math::Vector2 desiredVel = JimmyGod::Math::Normalize(agent.Destination - agent.Position) * agent.MaxSpeed;
		desiredVel -= agent.Velocity;
		if (IsDebugUIActive())
		{
			JimmyGod::Graphics::SimpleDraw::AddScreenLine(agent.Destination, agent.Position, JimmyGod::Graphics::Colors::Aqua);
			JimmyGod::Graphics::SimpleDraw::AddScreenCircle(JimmyGod::Math::Circle{ agent.Destination,5.0f }, JimmyGod::Graphics::Colors::Yellow);
		}
		return desiredVel;
	}
	return JimmyGod::Math::Vector2();
}
