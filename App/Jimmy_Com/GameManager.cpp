#include "GameManager.h"

#include "Flag.h"
#include "Unit.h"
#include "Soldier.h"
#include "Mutant.h"
#include "GridManager.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;

namespace
{
	std::unique_ptr<GameManager> sInstance;
}

void GameManager::StaticInitialize(size_t count)
{
	sInstance = std::make_unique<GameManager>();
	sInstance->Initialize(count);
}

void GameManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

GameManager* GameManager::Get()
{
	return sInstance.get();
}

void GameManager::Initialize(size_t count)
{
	mCamera = mWorld.AddService<CameraService>();
	mRender = mWorld.AddService<RenderService>();

	mWorld.Initialize(count);
	auto& camera = mCamera->GetActiveCamera();

	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(1750.0f);
	camera.SetPosition({ 0.0f, 70.0f, -1.0f });
	camera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mRender->Initialize();


}

void GameManager::Terminate()
{
	mRender->Terminate();
	mWorld.Terminate();
}

void GameManager::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	auto& camera = mCamera->GetActiveCamera();
	mRender->Update(deltaTime);

	mMouseX = inputSystem->GetMouseScreenX();
	mMouseY = inputSystem->GetMouseScreenY();

	mRay = camera.ScreenPointToWorldRay(mMouseX, mMouseY);
}

void GameManager::Render()
{
}

void GameManager::DebugUI()
{
}

void GameManager::Spawn(const JimmyGod::Math::Vector3& pos, const char* name, UnitType type, Flag flag)
{

}

bool GameManager::RayCast(const JimmyGod::Math::Ray& mousePoint, const JimmyGod::Math::Vector3& target, float maxDistance, Flag layerMask)
{
	if ((Distance(mousePoint.GetPoint(80.0f), target) < maxDistance))
	{
	}


	return false;
}

const Unit* GameManager::GetTarget(const JimmyGod::Math::Vector3& pos) const
{
	// TODO: insert return statement here
	return nullptr;
}
