#pragma once

#include "Agent.h"
#include "PartitionGrid.h"
namespace JimmyGod::AI
{
	class AIWorld
	{
	public:
		struct Settings
		{
			JimmyGod::Math::Vector2 worldSize = {0.0f, 0.0f};
			float partitionGridSize = 0.0f;
		};
		struct WallSettings
		{
			JimmyGod::Math::LineSegment line;
			bool diagonal = false;
			float degree = 0.0f;
		};

		using Obstacles = std::vector<JimmyGod::Math::Circle>;
		using Walls = std::vector<WallSettings>;

		void Initialize(const Settings& settings);
		void Update();
		void Render();
		void RegisterEntity(Entity* entity);
		void UnregisterEntity(Entity* entity);

		uint32_t GetNextId() 
		{
			ASSERT(mNextId < UINT32_MAX, "Run out of ids");
			return mNextId++; 
		}

		void AddObstacles(const JimmyGod::Math::Circle& obstacles);
		void AddWalls(const JimmyGod::Math::LineSegment& walls, bool dia = false, float degree = 0.0f);

		EntityList GetEntities(const JimmyGod::Math::Circle& range, int typeId);
		AgentList GetNeighborhood(const JimmyGod::Math::Circle& range, int typeId);

		void DebugDraw() const;

		const Obstacles& GetObstacles() const { return mObstacles; }
		const Walls& GetWalls() const { return mWalls; }

		bool HasLineOfSight(const JimmyGod::Math::Vector2& start, const JimmyGod::Math::Vector2& end);

	private:
		Settings mSetting;
		Obstacles mObstacles;
		Walls mWalls;
		EntityList mEntityList;
		PartitionGrid<Entity> mPartitionGrid;
		uint32_t mNextId = 0;

		Graphics::TextureId mRockTexture[3];
	};
}