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
	for (auto& unit : mSoliders)
	{
		unit->Terminate();
		unit.reset();
	}
	for (auto& unit : mMutants)
	{
		unit->Terminate();
		unit.reset();
	}
}

void GameManager::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	auto& camera = mCamera->GetActiveCamera();
	mWorld.Update(deltaTime);
	mRender->Update(deltaTime);

	TurnProcess();

	mMouseX = inputSystem->GetMouseScreenX();
	mMouseY = inputSystem->GetMouseScreenY();

	mRay = camera.ScreenPointToWorldRay(mMouseX, mMouseY);

	if (inputSystem->IsMousePressed(MouseButton::LBUTTON))
	{
		for (auto& gameObject : mSoliders)
		{
			if (RayCast(mRay, gameObject->GetUnit(), 0.0f, mCurrentState) && gameObject->GetAct() == false)
			{
				mUnit = &gameObject->GetUnit();
				GridManager::Get()->GetGird().ObjectPosition(mUnit->GetAgent().GetPosition());
				break;
			}
		}

		for (auto& gameObject : mMutants)
		{
			if (RayCast(mRay, gameObject->GetUnit(), 0.0f, mCurrentState) && gameObject->GetAct() == false)
			{
				mUnit = &gameObject->GetUnit();
				GridManager::Get()->GetGird().ObjectPosition(mUnit->GetAgent().GetPosition());
				break;
			}
		}
	}

	if(inputSystem->IsMousePressed(MouseButton::LBUTTON) && mUnit)
	{
		for (auto& node : GridManager::Get()->GetGraph().GetNodes())
		{
			if (RayCast(mRay, node.collider, 0.0f))
			{
				bool check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(node.coordinate);
				if (check)
				{
					mDestination = node.coordinate;
					GridManager::Get()->GetGird().ObjectPosition(node.position);
				}
				break;
			}
		}
	}

	if (inputSystem->IsKeyDown(KeyCode::SPACE) && mUnit)
	{
		mActive = true;
		mUnit->Move(mDestination);
	}

	if (mUnit)
	{
		if (mActive == true && mUnit->GetAgent().mPath.size() == 0)
		{
			mActive = false;
			GridManager::Get()->GetGird().ObjectPosition(mUnit->GetAgent().GetPosition());
			mDestination = GridManager::Get()->GetGraph().GetNode(mUnit->GetAgent().GetPosition())->coordinate;
		}
	}

	if (inputSystem->IsKeyPressed(KeyCode::ENTER))
	{
		mUnit->SetAct(true);
		mUnit = nullptr;
	}

	for (auto& unit : mSoliders)
		unit->Update(deltaTime);
	for (auto& unit : mMutants)
		unit->Update(deltaTime);
}

void GameManager::Render()
{
	auto matView = mCamera->GetActiveCamera().GetViewMatrix();
	auto matProj = mCamera->GetActiveCamera().GetPerspectiveMatrix();
	mWorld.Render();

	mRender->Render();

	for (auto& unit : mSoliders)
	{
		unit->Render(mCamera->GetActiveCamera());
	}
	for (auto& unit : mMutants)
	{
		unit->Render(mCamera->GetActiveCamera());
	}

	SimpleDraw::AddGroundPlane(80.0f, 2.0f, Colors::Black);
	SimpleDraw::Render(mCamera->GetActiveCamera());
}

void GameManager::DebugUI()
{
	mWorld.DebugUI();
	if (mUnit != nullptr)
	{
		if (mUnit->GetAgent().GetSpeed() == 0)
			GridManager::Get()->GetGird().DisplayAreaCube(
				mUnit->GetAgent().mArea,
				mUnit->GetAgent().GetPosition(), GetColor(mCurrentState));
	}
	GridManager::Get()->GetGird().DebugUI();
}

void GameManager::Spawn(const JimmyGod::Math::Vector3& pos, const char* name, UnitType type, Flag flag)
{
	switch (type)
	{
	case UnitType::Soldier:
	{
		auto& newUnit = mSoliders.emplace_back(new Soldier(name, flag));
		newUnit->Initialize(&mWorld);
		newUnit->GetAgent().GetTransformComponent().SetPosition(pos);
	} break;
	case UnitType::Mutant:
	{
		auto& newUnit = mMutants.emplace_back(new Mutant(name, flag));
		newUnit->Initialize(&mWorld);
		newUnit->GetAgent().GetTransformComponent().SetPosition(pos);
	} break;
	default: break;
	}
}

const JimmyGod::Graphics::Color GameManager::GetColor(Flag flag) const
{
	switch (flag)
	{
	case Flag::Ally:
		return Colors::Green;
		break;
	case Flag::Neutral:
		return Colors::Yellow;
		break;
	case Flag::Enemy:
		return Colors::Red;
		break;
	default:
		break;
	}
	return Vector4();
}

bool GameManager::RayCast(const JimmyGod::Math::Ray& mousePoint, const Unit& collider, float maxDistance, Flag layerMask)
{
	return ((layerMask == collider.GetFlag()) && Intersect(mousePoint, collider.GetSphereCollider(), maxDistance));
}

bool GameManager::RayCast(const JimmyGod::Math::Ray& mousePoint, const JimmyGod::Math::Sphere& collider, float maxDistance)
{
	return (Intersect(mousePoint, collider, maxDistance));
}

bool GameManager::TurnProcess()
{
	for (auto& unit : mSoliders)
	{
		if (unit->GetFlag() == mCurrentState && unit->GetAct() == false) return false;
	}
	for (auto& unit : mMutants)
	{
		if (unit->GetFlag() == mCurrentState && unit->GetAct() == false) return false;
	}

	switch (mCurrentState)
	{
	case Flag::Ally:
		mCurrentState = Flag::Neutral;
		break;
	case Flag::Neutral:
		mCurrentState = Flag::Enemy;
		break;
	case Flag::Enemy:
		mCurrentState = Flag::Ally;
		break;
	default:
		break;
	}

	for (auto& unit : mSoliders)
		unit->SetAct(false);
	for (auto& unit : mMutants)
		unit->SetAct(false);

	return true;
}
