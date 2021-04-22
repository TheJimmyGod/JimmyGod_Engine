#include "Precompiled.h"
#include "SeekBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 SeekBehavior::Calculate(Agent & agent)
{
	if (IsActive())
	{
		if (IsDebugUIActive())
		{
			JimmyGod::Graphics::SimpleDraw::AddScreenLine(agent.Destination, agent.Position, JimmyGod::Graphics::Colors::Aqua);
			JimmyGod::Graphics::SimpleDraw::AddScreenCircle(JimmyGod::Math::Circle{ agent.Destination,10.0f }, JimmyGod::Graphics::Colors::Aqua);
		}
		return Seek(agent, agent.Destination);
	}
	else
	{
		return JimmyGod::Math::Vector2();
	}
}

JimmyGod::Math::Vector2 SeekBehavior::Seek(Agent & agent, JimmyGod::Math::Vector2 destination)
{
	return ((JimmyGod::Math::Normalize(destination - agent.Position) *agent.MaxSpeed) - agent.Velocity);
}
