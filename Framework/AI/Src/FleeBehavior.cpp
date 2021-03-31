#include "Precompiled.h"
#include "FleeBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 FleeBehavior::Calculate(Agent & agent)
{
	if (IsActive())
	{
		const double PanicDistanceSq = 500.0f * 500.0f;
		if (JimmyGod::Math::DistanceSqr(agent.Position, agent.Destination) > PanicDistanceSq)
			return JimmyGod::Math::Vector2{ 0,0 };

		return ((JimmyGod::Math::Normalize(agent.Position - agent.Destination) *agent.MaxSpeed) - agent.Velocity);
	}
	else
	{
		return JimmyGod::Math::Vector2();
	}
}
