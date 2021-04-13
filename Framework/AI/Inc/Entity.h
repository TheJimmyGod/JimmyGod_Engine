#pragma once

namespace JimmyGod::AI
{
	class AIWorld;
	class Entity;

	using EntityList = std::vector<Entity*>;


	class Entity
	{
	public:
		// & == *const
		Entity(JimmyGod::AI::AIWorld& world, uint16_t typeId);
		virtual ~Entity();

		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;
		Entity(Entity &&) = delete;
		Entity& operator=(Entity&&) = delete;

		JimmyGod::Math::Matrix3 LocalToWorld() const;
		const uint32_t GetTypeId() const { return mUniqueId >> 32; }
		const uint64_t GetUniqueId() const { return mUniqueId; }
		AIWorld& world;


		float Radius = 1.0f;
		JimmyGod::Math::Vector2 Position = JimmyGod::Math::Vector2::Zero;
		JimmyGod::Math::Vector2 Heading = JimmyGod::Math::Vector2::YAxis;
	private:
		const uint64_t mUniqueId = 0;

	};

	using Entities = std::vector<Entity*>;


}

// && = Rvalue reference