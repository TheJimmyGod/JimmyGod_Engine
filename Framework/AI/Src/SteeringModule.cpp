#include "Precompiled.h"
#include "SteeringModule.h"

using namespace JimmyGod::AI;

SteeringModule::SteeringModule(Agent& agent)
	: mAgent(agent)
{}

JimmyGod::Math::Vector2 SteeringModule::Calculate()
{
	JimmyGod::Math::Vector2 total;
	for (auto& [name, behavior] : mBehaviors)
	{
		if (behavior->IsActive())
		{
			total += behavior->Calculate(mAgent);
		}
	}

	return total;
}