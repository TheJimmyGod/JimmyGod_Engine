#include "Precompiled.h"
#include "FleeBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 FleeBehavior::Calculate(Agent & agent)
{
	if (IsActive())
	{
		const float PanicDistanceSq = panicDistance * panicDistance;
		if (JimmyGod::Math::DistanceSqr(agent.Position, agent.Destination) > PanicDistanceSq)
			return JimmyGod::Math::Vector2{ 0,0 };
		if (IsDebugUIActive())
		{
			JimmyGod::Graphics::SimpleDraw::AddScreenLine(agent.Destination, agent.Position, JimmyGod::Graphics::Colors::Aqua);
			JimmyGod::Graphics::SimpleDraw::AddScreenCircle(JimmyGod::Math::Circle{ agent.Destination,10.0f },JimmyGod::Graphics::Colors::Aqua);
		}
		return ((JimmyGod::Math::Normalize(agent.Position - agent.Destination) *agent.MaxSpeed) - agent.Velocity);
	}
	else
	{
		return JimmyGod::Math::Vector2();
	}
}
