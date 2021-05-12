#include "Precompiled.h"
#include "AIWorld.h"

using namespace JimmyGod::AI;

namespace // internal link
{
	template<class Element>
	std::vector<Element*> GetElements(const PartitionGrid<Entity>& gird, const JimmyGod::Math::Circle& range, float cellsize, uint32_t typeId)
	{
		std::vector<Element*> elements;

		int minX = static_cast<int>((range.center.x - range.radius) / cellsize);
		int maxX = static_cast<int>((range.center.x + range.radius) / cellsize);
		int minY = static_cast<int>((range.center.y - range.radius) / cellsize);
		int maxY = static_cast<int>((range.center.y + range.radius) / cellsize);

		minX = JimmyGod::Math::Max(minX, 0);
		maxX = JimmyGod::Math::Min(maxX, gird.GetColumns() -1 );
		minY = JimmyGod::Math::Max(minY, 0);
		maxY = JimmyGod::Math::Min(maxY, gird.GetRows() - 1);
		
		for (int y = 0; y <= maxY; ++y)
		{
			for (int x = 0; x <= maxX; ++x)
			{
				auto& cell = gird.GetCell(x, y);
				for (auto& element : cell)
				{
					if (element->GetTypeId() == typeId)
						elements.push_back(static_cast<Element*>(element));
				}
			}
		}

		return elements;
	}
}


void AIWorld::Initialize(const Settings& settings)
{
	mSetting = settings;
	ASSERT(settings.partitionGridSize > 0.0f, "[AIWorld] Partition gird size must be greater than zero");
	const int columns = static_cast<int>(std::ceilf(settings.worldSize.x / settings.partitionGridSize));
	const int rows = static_cast<int>(std::ceilf(settings.worldSize.y / settings.partitionGridSize));
	mPartitionGrid.Resize(columns, rows);

	mRockTexture[0] = Graphics::TextureManager::Get()->Load("Rock1.png");
	mRockTexture[1] = Graphics::TextureManager::Get()->Load("Rock2.png");
	mRockTexture[2] = Graphics::TextureManager::Get()->Load("Rock3.png");
}

void AIWorld::Update()
{
	mPartitionGrid.ClearCells();
	for (auto entity : mEntityList)
	{
		int column = static_cast<int>(entity->Position.x / mSetting.partitionGridSize);
		int row = static_cast<int>(entity->Position.y / mSetting.partitionGridSize);
		mPartitionGrid.GetCell(column, row).push_back(entity);
	}
}

void JimmyGod::AI::AIWorld::Render()
{
	for (auto& wall : mWalls)
	{
		if (!wall.diagonal)
		{
			Graphics::SpriteRenderManager::Get()->DrawSprite(mRockTexture[2], (wall.line.from + wall.line.to) * 0.5f, wall.degree); // 89.5f
		}
		else
		{
			Graphics::SpriteRenderManager::Get()->DrawSprite(mRockTexture[2], (wall.line.from + wall.line.to) * 0.5f, wall.degree); //-6.95f
		}
	}
	for (auto& rock : mObstacles)
	{
		if(rock.radius == 80.0f)
			Graphics::SpriteRenderManager::Get()->DrawSprite(mRockTexture[0], rock.center);
		else if(rock.radius == 50.0f)
			Graphics::SpriteRenderManager::Get()->DrawSprite(mRockTexture[1], rock.center);
	}

}

void AIWorld::RegisterEntity(Entity * entity)
{
	mEntityList.push_back(entity);
}

void AIWorld::UnregisterEntity(Entity * entity)
{
	auto iter = std::find(mEntityList.begin(), mEntityList.end(), entity);
	if (iter != mEntityList.end())
	{
		std::iter_swap(iter, mEntityList.end() - 1);
		mEntityList.pop_back();
	}
}

void AIWorld::Clear()
{
	mObstacles.clear();
	mWalls.clear();
}

void AIWorld::AddObstacles(const JimmyGod::Math::Circle & obstacles)
{
	mObstacles.push_back(obstacles);
}

void AIWorld::AddWalls(const JimmyGod::Math::LineSegment & walls, bool dia, float degree)
{
	mWalls.push_back({ walls,dia, degree });
}

EntityList AIWorld::GetEntities(const JimmyGod::Math::Circle & range, int typeId)
{
	return GetElements<Entity>(mPartitionGrid, range, mSetting.partitionGridSize, typeId);
}

AgentList AIWorld::GetNeighborhood(const JimmyGod::Math::Circle & range, int typeId)
{
	return GetElements<Agent>(mPartitionGrid, range, mSetting.partitionGridSize, typeId);
}

void AIWorld::DebugDraw(bool avoidance) const
{
	if (avoidance)
	{
		if (mObstacles.size() > 0)
		{
			for (auto& obstacle : mObstacles)
			{
				JimmyGod::Graphics::SimpleDraw::AddScreenCircle(Circle(obstacle.center, obstacle.radius), JimmyGod::Graphics::Colors::Cyan);
			}
		}
		if (mWalls.size() > 0)
		{
			for (auto& wall : mWalls)
			{
				JimmyGod::Graphics::SimpleDraw::AddScreenLine(wall.line.from, wall.line.to, JimmyGod::Graphics::Colors::Cyan);
			}
		}
	}


	for (int x = 0; x < mPartitionGrid.GetColumns(); x++)
	{
		JimmyGod::Graphics::SimpleDraw::AddScreenLine({ x*mSetting.partitionGridSize,0 }, { x*mSetting.partitionGridSize, static_cast<float>(JimmyGod::Graphics::GraphicsSystem::Get()->GetBackBufferHeight()) }, JimmyGod::Graphics::Colors::White);
	}
	for (int y = 0; y < mPartitionGrid.GetRows(); y++)
	{
		JimmyGod::Graphics::SimpleDraw::AddScreenLine({ 0,y*mSetting.partitionGridSize }, { static_cast<float>(JimmyGod::Graphics::GraphicsSystem::Get()->GetBackBufferWidth()), y*mSetting.partitionGridSize }, JimmyGod::Graphics::Colors::White);
	}
}

bool AIWorld::HasLineOfSight(const JimmyGod::Math::Vector2 & start, const JimmyGod::Math::Vector2 & end)
{
	JimmyGod::Math::LineSegment line{ start, end };

	for (auto& wall : mWalls)
	{
		if (JimmyGod::Math::Intersect(line, wall.line))
			return false;
	}

	for (auto& obstacle : mObstacles)
	{
		if (JimmyGod::Math::Intersect(line, obstacle))
			return false;
	}

	return true;
}
