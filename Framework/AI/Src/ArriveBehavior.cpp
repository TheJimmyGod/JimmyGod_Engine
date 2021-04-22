#include"Precompiled.h"
#include"ArriveBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 ArriveBehavior::Calculate(Agent & agent)
{
	if (IsActive())
	{
		if (IsDebugUIActive())
		{
			JimmyGod::Graphics::SimpleDraw::AddScreenCircle(JimmyGod::Math::Circle{ agent.Destination,10.0f },
				JimmyGod::Graphics::Colors::Aqua);
			JimmyGod::Graphics::SimpleDraw::AddScreenLine(agent.Destination, agent.Position, JimmyGod::Graphics::Colors::Aqua);
		}
		return Arrive(agent, agent.Destination, 3.0f, 0.6f);
	}
	else
	{
		return JimmyGod::Math::Vector2{ 0,0 };
	}
}

JimmyGod::Math::Vector2 JimmyGod::AI::ArriveBehavior::Arrive(Agent & agent, JimmyGod::Math::Vector2 destination
, float decel, float decelTweaker)
{
	JimmyGod::Math::Vector2 toTarget = destination - agent.Position;
	float distance = JimmyGod::Math::Magnitude(toTarget);
	JimmyGod::Math::Vector2 velocity = Vector2::Zero;

	if (distance < 200.0f)
	{
		float speed = distance / decel * decelTweaker;
		speed = JimmyGod::Math::Min(speed, agent.MaxSpeed);
		velocity = toTarget * speed / distance;
	}
	else
	{
		velocity = toTarget * agent.MaxSpeed / distance;
	}
	return velocity - agent.Velocity;
}
