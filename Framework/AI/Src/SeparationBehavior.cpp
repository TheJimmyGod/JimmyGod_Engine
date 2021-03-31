#include "Precompiled.h"
#include "SeparationBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 JimmyGod::AI::SeparationBehavior::Calculate(Agent & agent)
{
	JimmyGod::Math::Vector2 totalForce;
	for (auto& neighbor : agent.neighbors)
	{
		auto neighborToAgent = agent.Position - neighbor->Position;
		auto distanceToAgent = JimmyGod::Math::Magnitude(neighborToAgent);
		if (distanceToAgent <= 0.0f)
		{
			totalForce += agent.Heading * agent.MaxSpeed;
		}
		else
		{
			auto seperationDirection = neighborToAgent / distanceToAgent;
			auto sepaerationForce = agent.MaxSpeed * (1.0f - JimmyGod::Math::Min(distanceToAgent, 100.0f) / 100.0f);
			totalForce += seperationDirection * sepaerationForce;
		}

	}
	return totalForce;
}
