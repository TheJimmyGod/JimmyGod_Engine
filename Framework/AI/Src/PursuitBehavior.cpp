#include "Precompiled.h"
#include "PursuitBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 PursuitBehavior::Calculate(Agent& agent)
{

	JimmyGod::Math::Vector2 distance = agent.threat->Position + agent.threat->Velocity - agent.Position;
	JimmyGod::Math::Vector2 futurePosition = JimmyGod::Math::Normalize(distance) * agent.MaxSpeed;
	return futurePosition - agent.Velocity;
}
