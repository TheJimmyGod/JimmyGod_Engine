#pragma once
#include "Entity.h"
namespace JimmyGod::AI
{
	class Agent;

	using AgentList = std::vector<Agent*>;

	class Agent : public JimmyGod::AI::Entity
	{
	public:
		Agent(AIWorld& world, uint32_t typeId);
		~Agent() override = default;

		Agent* threat = nullptr;

		AgentList neighbors;

		JimmyGod::Math::Vector2 Velocity = JimmyGod::Math::Vector2::Zero;
		JimmyGod::Math::Vector2 Destination = JimmyGod::Math::Vector2::YAxis;
		float MaxSpeed = 1.0f;
		float Mass = 1.0f;
	private:

	};


}