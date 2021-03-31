#include "Precompiled.h"
#include "WallAvoidBehavior.h"

using namespace JimmyGod::AI;

JimmyGod::Math::Vector2 WallAvoidBehvior::Calculate(Agent & agent)
{

	JimmyGod::Math::Vector2 direction = JimmyGod::Math::Normalize(agent.Velocity);

	auto forward = agent.Position + direction * 150.0f;
	auto halfForward = agent.Position + direction * 75.0f;


	float dist = 0.0f;
	float minDist = std::numeric_limits<float>::infinity();
	bool collision = false;
	JimmyGod::Math::Vector2 steeringForce, point = JimmyGod::Math::Vector2();
	JimmyGod::Math::LineSegment nearest;
	for (auto& wall : agent.world.GetWalls())
	{
		JimmyGod::Math::Vector2 iter = GetDistanceWall(agent.Position, wall);
		dist = GetDistance(agent.Position, wall);

		if (dist < minDist)
		{
			JimmyGod::Math::Vector2 heading = iter - agent.Position;
			float angle = JimmyGod::Math::Dot(heading, direction);

			if (angle > 0.0f)
			{
				nearest = wall;
				collision = LineIntersectsWall(agent, forward, halfForward, nearest);
				minDist = dist;

				point = iter;
				break;
			}
			
		}
	}
	
	if (collision)
	{
		point = GetDistanceWall(agent.Position, nearest);
		JimmyGod::Graphics::SimpleDraw::AddScreenCircle(Circle(Vector2(nearest.to.x, nearest.from.y), 100.0f), JimmyGod::Graphics::Colors::Yellow);
		steeringForce = forward - point;
		steeringForce = JimmyGod::Math::Normalize(steeringForce);
		steeringForce *= agent.MaxSpeed;
		return steeringForce;
	}
	else
	{
		JimmyGod::Graphics::SimpleDraw::AddScreenCircle(Circle(Vector2(nearest.from.x, nearest.from.y), 3.5f), JimmyGod::Graphics::Colors::Magenta);
	}
	

	return JimmyGod::Math::Vector2{ 0.0f, 0.0f };
}
