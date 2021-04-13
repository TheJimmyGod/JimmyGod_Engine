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
	if (agent.neighbors.size() == 0)
	{
		averageHeading /= 1.0f;
	}
	else
	{
		averageHeading /= (agent.neighbors.size());
	}
	
	averageHeading -= agent.Heading;
	//averageHeading = X::Math::Normalize(averageHeading);
	return averageHeading;
}
