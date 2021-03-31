#include "Precompiled.h"
#include "AlignmentBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 JimmyGod::AI::AlignmentBehavior::Calculate(Agent & agent)
{
	JimmyGod::Math::Vector2 averageHeading = agent.Heading;

	for (auto& neighbor : agent.neighbors)
	{
		averageHeading += neighbor->Heading;
	}
	averageHeading /= (agent.neighbors.size() + 1.0f);
	averageHeading -= agent.Heading;
	//averageHeading = X::Math::Normalize(averageHeading);
	return averageHeading;
}
