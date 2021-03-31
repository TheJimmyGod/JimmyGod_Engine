#include "Precompiled.h"
#include "SeekBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 SeekBehavior::Calculate(Agent & agent)
{
	if (IsActive())
	{
		return Seek(agent, agent.Destination);
	}
	else
	{
		return JimmyGod::Math::Vector2();
	}
}

JimmyGod::Math::Vector2 SeekBehavior::Seek(Agent & agent, JimmyGod::Math::Vector2 destination)
{
	
	return ((JimmyGod::Math::Normalize(agent.Destination - agent.Position) *agent.MaxSpeed) - agent.Velocity);
}
