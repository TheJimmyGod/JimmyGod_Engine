#include "Environment.h"
#include "GridManager.h"

using namespace JimmyCom;
using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

JimmyCom::Environment::Environment(std::string name, bool destructible):
	mName(name), mDestructible(destructible)
{
	mTransformBuffer.Initialize();
	mActive = true;
}

void JimmyCom::Environment::Initialize(JimmyGod::GameWorld* gameWorld, std::filesystem::path& path)
{
	ASSERT(gameWorld != nullptr, "The Game World does not exist!");
	gameWorld->Create(path, mName);
	mGameObject = gameWorld->Find(mName).Get();
}

void JimmyCom::Environment::Terminate()
{
	mTransformBuffer.Terminate();
	mGameObject->Terminate();
}

void JimmyCom::Environment::Update(float deltaTime)
{
	mGameObject->GetComponent<ColliderComponent>()->Update(deltaTime);

}

void JimmyCom::Environment::Render(const JimmyGod::Graphics::Camera& camera)
{
	if (!mActive) return;

	auto matView = camera.GetViewMatrix();
	auto matProj = camera.GetPerspectiveMatrix();
	
	auto matWorld = mGameObject->GetComponent<TransformComponent>()->GetTransform();
	mGameObject->GetComponent<MeshComponent>()->Bind();
	mTransformBuffer.BindVS(0);
	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = camera.GetPosition();
	mTransformBuffer.Update(&transformData);
	mGameObject->GetComponent<MeshComponent>()->Render();
}

void JimmyCom::Environment::TakeDamage(float dmg)
{
	if (!mDestructible)
		return;
	if (mHealth > 0.0f)
		mHealth -= dmg;
	else
		Destory();
}

float JimmyCom::Environment::GetTerritory() const
{
	return mGameObject->GetComponent<ColliderComponent>()->radius;
}

void JimmyCom::Environment::InstallGrid()
{
	AI::Coord coord = GridManager::Get()->GetGrid().GetGraph().GetNode(mGameObject->GetComponent<TransformComponent>()->GetPosition())->coordinate;
	coord.y += static_cast<int>(GetTerritory());
	int minX, maxX, minY, maxY;
	minY = Max(coord.y - static_cast<int>(GetTerritory()), 1);
	minX = Max(coord.x - static_cast<int>(GetTerritory()), 1);
	maxY = Min(coord.y + static_cast<int>(GetTerritory()), GridManager::Get()->GetGrid().GetRows());
	maxX = Min(coord.x + static_cast<int>(GetTerritory()), GridManager::Get()->GetGrid().GetColumns());

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			const int index = GridManager::Get()->GetGird().GetIndex(x, y);
			if (GridManager::Get()->GetGird().GetGraph().GetNode(AI::Coord{ x,y }))
			{
				GridManager::Get()->GetGird().GetNodes()[index].SetWalkable(false);
			}
		}
	}
}
