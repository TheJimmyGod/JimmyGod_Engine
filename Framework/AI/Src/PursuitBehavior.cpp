#include "Precompiled.h"
#include "PursuitBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 PursuitBehavior::Calculate(Agent& agent)
{
	//JimmyGod::Math::Vector2 distance = agent.threat->Position + agent.threat->Velocity - agent.Position;
	//JimmyGod::Math::Vector2 futurePosition = JimmyGod::Math::Normalize(distance) * agent.MaxSpeed;
	//return futurePosition - agent.Velocity;

	JimmyGod::Math::Vector2 distance = agent.threat->Position - agent.Position;
	float relativeHeading = JimmyGod::Math::Dot(agent.Heading, agent.threat->Heading);
	if (IsDebugUIActive())
	{
		JimmyGod::Graphics::SimpleDraw::AddScreenCircle(JimmyGod::Math::Circle{ agent.threat->Position,10.0f },
			JimmyGod::Graphics::Colors::Aqua);
		JimmyGod::Graphics::SimpleDraw::AddScreenLine(agent.Position, agent.threat->Position, JimmyGod::Graphics::Colors::Aqua);
	}
	if ((JimmyGod::Math::Dot(distance, agent.Heading) > 0.0f) &&
		(relativeHeading < -0.95f))
	{
		agent.Destination = agent.threat->Position;
		return SeekBehavior::Seek(agent, agent.threat->Position);
	}
	float length = Magnitude(distance);
	float lookAheadTime = length / (agent.MaxSpeed + agent.threat->MaxSpeed);
	agent.Destination = agent.threat->Position + agent.threat->Velocity * lookAheadTime;
	return SeekBehavior::Seek(agent, agent.threat->Position + agent.threat->Velocity * lookAheadTime);
}
