#include "Precompiled.h"
#include "CohesionBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 CohesionBehavior::Calculate(Agent & agent)
{

	JimmyGod::Math::Vector2 averagePosition = agent.Position;
	for (auto& neighbor : agent.neighbors)
	{
		averagePosition += neighbor->Position;
	}
	averagePosition /= (agent.neighbors.size() + 1.0f);

	return Seek(agent, averagePosition);
}
