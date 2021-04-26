#include "Precompiled.h"
#include "WallAvoidBehavior.h"

using namespace JimmyGod;
using namespace JimmyGod::AI;
using namespace JimmyGod::Math;
Vector2 WallAvoidBehvior::Calculate(Agent & agent)
{
	float speed = JimmyGod::Math::Magnitude(agent.Velocity);
	float minDistance = std::numeric_limits<float>::infinity();
	float thisDistance = 0.0f;

	auto forward = agent.Position + JimmyGod::Math::Normalize(agent.Velocity) * 50.0f;
	auto halfForward = agent.Position + JimmyGod::Math::Normalize(agent.Velocity) * 25.0f;

	auto aheadVector1 = forward + JimmyGod::Math::Rotate(JimmyGod::Math::Normalize(agent.Velocity), 45.0f) * agent.Radius;
	auto aheadVector2 = forward + JimmyGod::Math::Rotate(JimmyGod::Math::Normalize(agent.Velocity), -45.0f) * agent.Radius;

	auto aheadVector3 = agent.Position + JimmyGod::Math::Normalize(agent.Velocity) + JimmyGod::Math::Rotate(JimmyGod::Math::Normalize(agent.Velocity), 45.0f) * agent.Radius;
	auto aheadVector4 = agent.Position + JimmyGod::Math::Normalize(agent.Velocity) + JimmyGod::Math::Rotate(JimmyGod::Math::Normalize(agent.Velocity), -45.0f) * agent.Radius;

	float length = agent.Radius + (speed / agent.MaxSpeed) * agent.Radius;

	auto ahead = agent.Position + JimmyGod::Math::Normalize(agent.Velocity) * length;
	ahead = JimmyGod::Math::Magnitude(ahead);

	Vector2 SteeringForce, point, closestPoint;

	int closestWallIndex = -1;
	for (size_t i = 0; i < agent.world.GetWalls().size(); ++i)
	{
		bool isHalf = false;
		if (Intersect(Circle{ forward,agent.Radius }, agent.world.GetWalls()[i], &closestPoint))
		{
			thisDistance = Distance(agent.world.GetWalls()[i], forward);
			minDistance = thisDistance;
			closestWallIndex = i;
		}
		else if (Intersect(Circle{ halfForward,agent.Radius }, agent.world.GetWalls()[i], &closestPoint))
		{
			thisDistance = Distance(agent.world.GetWalls()[i], halfForward);
			minDistance = thisDistance;
			closestWallIndex = i;
		}

		if (closestWallIndex >= 0)
		{
			Vector2 overShoot = isHalf ? (halfForward - closestPoint) : (forward - closestPoint);
			float cross = Cross(agent.world.GetWalls()[closestWallIndex].to, agent.world.GetWalls()[closestWallIndex].from);
			SteeringForce = overShoot.x < 0.0f ? (PerpendicularLH(overShoot) * cross) : (PerpendicularRH(overShoot) * cross);
		}
	}

	if (IsDebugUIActive())
	{
		JimmyGod::Graphics::SimpleDraw::AddScreenLine(aheadVector1, aheadVector3, JimmyGod::Graphics::Colors::Aqua);
		JimmyGod::Graphics::SimpleDraw::AddScreenLine(aheadVector4, aheadVector2, JimmyGod::Graphics::Colors::Aqua);
		JimmyGod::Graphics::SimpleDraw::AddScreenLine(aheadVector1, aheadVector2, JimmyGod::Graphics::Colors::Aqua);
		JimmyGod::Graphics::SimpleDraw::AddScreenLine(aheadVector3, aheadVector4, JimmyGod::Graphics::Colors::Aqua);
	}

	return SteeringForce;
}
