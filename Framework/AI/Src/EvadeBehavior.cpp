#include "Precompiled.h"
#include "EvadeBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 EvadeBehavior::Calculate(Agent & agent)
{
	JimmyGod::Math::Vector2 distance = agent.threat->Position - agent.Position;
	const float ahead = JimmyGod::Math::Magnitude(distance);
	float distanceAhead = ahead / agent.MaxSpeed;

	agent.Destination = agent.threat->Position + agent.threat->Velocity* distanceAhead;

	JimmyGod::Math::Vector2 desiredVel = JimmyGod::Math::Normalize(agent.Position - agent.Destination) * agent.MaxSpeed;
	if (IsDebugUIActive())
	{
		JimmyGod::Graphics::SimpleDraw::AddScreenLine(agent.threat->Position, agent.Position, JimmyGod::Graphics::Colors::Red);
	}
	return desiredVel - agent.Velocity;
}
