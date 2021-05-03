#include "Precompiled.h"
#include "EnforceNonPenetrationConstraint.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 EnforceNonPenetrationConstraint::Calculate(Agent & agent)
{
	for (auto& entity : agent.neighbors)
	{
		if (entity == &agent)
			continue;
		Vector2 toEntity = agent.Position - entity->Position;
		float distFromEachOther = Magnitude(toEntity);
		float amountOfOverlap = entity->Radius + agent.Radius
			- distFromEachOther;
		if (amountOfOverlap >= 0.0f)
		{
			agent.Position = (agent.Position + (toEntity / distFromEachOther)
				* amountOfOverlap);
		}
		
	}
	if(IsDebugUIActive())
		JimmyGod::Graphics::SimpleDraw::AddScreenCircle(JimmyGod::Math::Circle{ agent.Position,agent.Radius }, JimmyGod::Graphics::Colors::Aqua);
	return Vector2::Zero;
}