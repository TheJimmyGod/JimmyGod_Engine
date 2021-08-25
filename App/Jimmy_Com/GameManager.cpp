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
	for (auto& unit : mSoldiers)
	{
		unit->Terminate();
		unit.reset();
	}
	for (auto& unit : mMutants)
	{
		unit->Terminate();
		unit.reset();
	}
	for (auto& item : mAlly)
		item.reset();
	for (auto& item : mEnemy)
		item.reset();

	mSoldiers.clear();
	mMutants.clear();

	mAlly.clear();
	mEnemy.clear();
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

	if (inputSystem->IsMousePressed(MouseButton::LBUTTON) && !isAttacking)
	{
		for (auto& gameObject : mAlly)
		{
			if (RayCast(mRay, gameObject->GetUnit(), mMaxDistance, mCurrentState) && gameObject->GetStatus() == Status::Standby)
			{
				mUnit = &gameObject->GetUnit();
				GridManager::Get()->GetGird().ObjectPosition(mUnit->GetAgent().GetPosition());
				break;
			}
		}

		for (auto& gameObject : mEnemy)
		{
			if (RayCast(mRay, gameObject->GetUnit(), mMaxDistance, mCurrentState) && gameObject->GetStatus() == Status::Standby)
			{
				mUnit = &gameObject->GetUnit();
				GridManager::Get()->GetGird().ObjectPosition(mUnit->GetAgent().GetPosition());
				break;
			}
		}
	}

	if(inputSystem->IsMousePressed(MouseButton::LBUTTON) && mUnit && !isAttacking)
	{
		for (auto& gameObject : mAlly)
		{
			if (&gameObject->GetUnit() == mUnit)
				continue;
			if (RayCast(mRay, gameObject->GetUnit(), mMaxDistance, Flag::Ally))
			{
				auto node = GridManager::Get()->GetGraph().GetNode(gameObject->GetAgent().GetPosition());
				bool check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(node->coordinate);
				if (check)
				{
					mTarget = (gameObject->GetFlag() != mUnit->GetFlag() ? &gameObject->GetUnit() : nullptr);
					if(mTarget)
						GridManager::Get()->GetGird().ObjectPosition(mTarget->GetAgent().GetPosition());
					break;
				}
			}
			else
				mTarget = nullptr;
		}

		for (auto& gameObject : mEnemy)
		{
			if (&gameObject->GetUnit() == mUnit)
				continue;
			if (RayCast(mRay, gameObject->GetUnit(), mMaxDistance, Flag::Enemy))
			{
				auto node = GridManager::Get()->GetGraph().GetNode(gameObject->GetAgent().GetPosition());
				bool check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(node->coordinate);
				if (check)
				{
					mTarget = (gameObject->GetFlag() != mUnit->GetFlag() ? &gameObject->GetUnit() : nullptr);
					if(mTarget)
						GridManager::Get()->GetGird().ObjectPosition(mUnit->GetAgent().GetPosition());
					break;
				}
			}
			else
				mTarget = nullptr;
		}

		if (!mTarget)
		{
			for (auto& node : GridManager::Get()->GetGraph().GetNodes())
			{
				if (RayCast(mRay, node.collider, mMaxDistance))
				{
					bool check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(node.coordinate);
					if (check)
					{
						mDestination = node.coordinate;
						GridManager::Get()->GetGird().ObjectPosition(node.position);
						break;
					}
				}
			}
		}

	}

	if (inputSystem->IsKeyDown(KeyCode::SPACE) && mUnit)
	{
		if (mTarget && mUnit->GetStatus() == Status::Standby)
		{
			mAttackActive = true;
			mUnit->SetStatus(Status::Attack);
		}
		else if (!mTarget && mUnit->GetStatus() == Status::Standby)
		{
			mMoveActive = true;
			mUnit->SetStatus(Status::Move);
			mUnit->Move(mDestination);
		}
		else if (mTarget && mUnit->GetStatus() == Status::Move)
		{
			mAttackActive = true;
			mUnit->SetStatus(Status::Attack);
		}
		else
			return;

	}

	if (mUnit)
	{
		switch (mUnit->GetStatus())
		{
		case Status::Move:
		{
			if (mUnit->GetAgent().mPath.size() == 0 && mMoveActive)
			{
				mMoveActive = false;
				GridManager::Get()->GetGird().ObjectPosition(mUnit->GetAgent().GetPosition());
				mDestination = GridManager::Get()->GetGraph().GetNode(mUnit->GetAgent().GetPosition())->coordinate;
				mUnit->SetStatus(Status::TurnOver);
				mUnit = nullptr;
				mTarget = nullptr;
			}
		}
			break;
		case Status::Attack:
		{
			if (mAttackActive)
			{
				mUnit->mTime = 2.5f;
				mUnit->GetAgent().GetTransformComponent().SetRotation(mUnit->GetAgent().GetPosition() -
					mTarget->GetAgent().GetPosition());
				mAttackActive = false;
				mUnit->GetAgent().GetModelComponent().SetAnimationTime(0.0f);
				mUnit->SetProcess(true);
				isAttacking = mUnit->GetProcess();
			}
			else
			{
				if (mUnit->AttackUpdate(deltaTime))
				{
					mUnit->SetProcess(false);
					isAttacking = mUnit->GetProcess();
					if(mTarget != nullptr)
						mUnit->Attack(*mTarget);
					mUnit->GetAgent().GetModelComponent().SetAnimationSpeed(1.0f);
					mTarget = nullptr;
					mUnit->SetStatus(Status::TurnOver);
					mUnit = nullptr;
				}
			}
		}
			break;
		default:
			break;
		}
	}

	for (auto& unit : mSoldiers)
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

	for (auto& unit : mSoldiers)
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
		if (mUnit->GetAgent().GetSpeed() == 0 && mUnit->GetStatus() == Status::Standby)
			GridManager::Get()->GetGird().DisplayAreaCube(
				mUnit->GetAgent().mArea,
				mUnit->GetAgent().GetPosition(), GetColor(mCurrentState));
		if (mUnit->GetStatus() != Status::TurnOver)
		{
			for (auto& unit : mAlly)
			{
				auto node = GridManager::Get()->GetGraph().GetNode(unit->GetAgent().GetPosition());
				bool check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(node->coordinate);
				if (check) JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(node->position, 1.0f), Colors::ForestGreen);
			}
			for (auto& unit : mEnemy)
			{
				auto node = GridManager::Get()->GetGraph().GetNode(unit->GetAgent().GetPosition());
				bool check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(node->coordinate);
				if (check) JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(node->position, 1.0f), Colors::OrangeRed);
			}
		}
	}
	GridManager::Get()->GetGird().DebugUI();
}

void GameManager::Spawn(const JimmyGod::Math::Vector3& pos, const char* name, UnitType type, Flag flag)
{
	std::shared_ptr<Unit> temp;
	switch (type)
	{
	case UnitType::Soldier:
	{
		auto& newUnit = mSoldiers.emplace_back(new Soldier(name, flag));
		newUnit->Initialize(&mWorld);
		newUnit->GetAgent().GetTransformComponent().SetPosition(pos);
		temp = newUnit;

	} break;
	case UnitType::Mutant:
	{
		auto& newUnit = mMutants.emplace_back(new Mutant(name, flag));
		newUnit->Initialize(&mWorld);
		newUnit->GetAgent().GetTransformComponent().SetPosition(pos);
		temp = newUnit;
	} break;
	}

	switch (flag)
	{
	case Flag::Ally:
		mAlly.push_back(temp);
		break;
	case Flag::Neutral:
		break;
	case Flag::Enemy:
		mEnemy.push_back(temp);
		break;
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

bool GameManager::RayCast(const JimmyGod::Math::Ray& mousePoint, const Unit& collider, float& maxDistance, Flag layerMask)
{
	return ((layerMask == collider.GetFlag()) && Intersect(mousePoint, collider.GetSphereCollider(), maxDistance));
}

bool GameManager::RayCast(const JimmyGod::Math::Ray& mousePoint, const JimmyGod::Math::Sphere& collider, float& maxDistance)
{
	return (Intersect(mousePoint, collider, maxDistance));
}

bool GameManager::TurnProcess()
{
	for (auto& unit : mSoldiers)
	{
		if (unit->GetFlag() == mCurrentState && unit->GetStatus() != Status::TurnOver) return false;
	}
	for (auto& unit : mMutants)
	{
		if (unit->GetFlag() == mCurrentState && unit->GetStatus() != Status::TurnOver) return false;
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

	for (auto& unit : mSoldiers)
		unit->SetStatus(Status::Standby);
	for (auto& unit : mMutants)
		unit->SetStatus(Status::Standby);

	return true;
}
