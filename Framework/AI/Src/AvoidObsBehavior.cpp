#include "Precompiled.h"
#include "AvoidObsBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 AvoidObsBehavior::Calculate(Agent & agent)
{
	float speed = JimmyGod::Math::Magnitude(agent.Velocity);
	float minDistance = std::numeric_limits<float>::infinity();

	auto forward = agent.Position + JimmyGod::Math::Normalize(agent.Velocity) * 100.0f;
	auto halfForward = agent.Position + JimmyGod::Math::Normalize(agent.Velocity) * 50.0f;

	auto aheadVector1 = forward + JimmyGod::Math::Rotate(JimmyGod::Math::Normalize(agent.Velocity), 45.0f) * agent.Radius;
	auto aheadVector2 = forward + JimmyGod::Math::Rotate(JimmyGod::Math::Normalize(agent.Velocity), -45.0f) * agent.Radius;

	auto aheadVector3 = agent.Position + JimmyGod::Math::Normalize(agent.Velocity) + JimmyGod::Math::Rotate(JimmyGod::Math::Normalize(agent.Velocity), 45.0f) * agent.Radius;
	auto aheadVector4 = agent.Position + JimmyGod::Math::Normalize(agent.Velocity) + JimmyGod::Math::Rotate(JimmyGod::Math::Normalize(agent.Velocity), -45.0f) * agent.Radius;

	float length = agent.Radius + (speed/agent.MaxSpeed) * agent.Radius;

	auto ahead = agent.Position + JimmyGod::Math::Normalize(agent.Velocity) * length;
	ahead = JimmyGod::Math::Magnitude(ahead);

	JimmyGod::Math::Matrix3 worldToLocal = JimmyGod::Math::Matrix3::Inverse(agent.LocalToWorld());
	
	JimmyGod::Math::Circle* nearest = nullptr;
	JimmyGod::Math::Vector2 force;
	JimmyGod::Math::Vector2 localObsPos = JimmyGod::Math::Vector2();
	JimmyGod::Math::Vector2 localObsNearestPos = JimmyGod::Math::Vector2();
	JimmyGod::Math::Vector2 worldPos = JimmyGod::Math::Vector2();
	float obsExpandRadius = 0.0f;

	for (auto& obs : agent.world.GetObstacles())
	{
		auto obstacle = obs;
		bool collision = lineIntersectsCircle(obstacle, forward, halfForward);
		if (collision)
		{
			localObsPos = JimmyGod::Math::TransformCoord(obstacle.center, worldToLocal);
			if (localObsPos.y >= 0.0f) // except for behind
			{
				obsExpandRadius = obstacle.radius + agent.Radius; // Expand remaining obstacles by width of bounding box
				if (fabs(localObsPos.x) < obsExpandRadius) // Eliminate obstacles whose abs(local y) < expanded radius
				{
					nearest = &obstacle;

					float cX = localObsPos.x; float cY = localObsPos.y;
					float sqrtPart = sqrtf(obsExpandRadius* obsExpandRadius - cX * cX);

					float ip = localObsPos.y - sqrtPart;

					if (ip <= 0.0f)
					{
						ip = localObsPos.y + sqrtPart;
					}

					worldPos = JimmyGod::Math::TransformCoord(ip, agent.LocalToWorld());

					if (ip < minDistance)
					{
						minDistance = ip;
						nearest = &obstacle;

						localObsNearestPos = localObsPos;
						break;
					}
				}
			}
		}
	}

	if (nearest)
	{
		float multiplier = 10.0f + (length - localObsNearestPos.y) / length;
		JimmyGod::Math::Vector2 steeringForce;
		steeringForce.x = (nearest->radius - localObsNearestPos.x) * multiplier;
		steeringForce.y = (nearest->radius - localObsNearestPos.y) * 0.15f;
		force = JimmyGod::Math::TransformCoord(steeringForce, agent.LocalToWorld());
		return force;
	}
	if (IsDebugUIActive())
	{
		JimmyGod::Graphics::SimpleDraw::AddScreenLine(aheadVector1, aheadVector3, JimmyGod::Graphics::Colors::Aqua);
		JimmyGod::Graphics::SimpleDraw::AddScreenLine(aheadVector4, aheadVector2, JimmyGod::Graphics::Colors::Aqua);
		JimmyGod::Graphics::SimpleDraw::AddScreenLine(aheadVector1, aheadVector2, JimmyGod::Graphics::Colors::Aqua);
		JimmyGod::Graphics::SimpleDraw::AddScreenLine(aheadVector3, aheadVector4, JimmyGod::Graphics::Colors::Aqua);
	}


	return JimmyGod::Math::Vector2();
}
// Physics Engine
// 1) World representation - proxy shapes(AABB, OBB, Sphere, Capsule, etc) - Particles(i.e. point mess) - Can be hierarchical - Can have constraints (e.g. joints)
// 2) Simulation - For object motion - Applies physics formulas(e.g. Kinematics) - Preserves rules of physics(e.g. Newton's Laws, Energy/Momentum conservation)
// 3) Collision Detection - Math/Geometry - Can be hierarchical(e.g. spatial partitioning, compound shapes)
// 4) Collision Resolution - Physics formulas - Preserves rules of physics(e.g. Newton's Laws, Energy/Momentum conservation)