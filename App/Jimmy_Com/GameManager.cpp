#include "GameManager.h"

#include "Flag.h"
#include "Unit.h"
#include "Soldier.h"
#include "Mutant.h"
#include "GridManager.h"

#define ATTACK_TIME 2.5f;

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyCom;

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

	mSoldiers.clear();
	mMutants.clear();
}

void GameManager::Update(float deltaTime)
{
	
	mWorld.Update(deltaTime);
	mRender->Update(deltaTime);

	TurnProcess();
	// Turn Process
	// -> The function that investigate status for each unit.
	ControlState();
	// Control State
	// -> The function that controls the unit with mouse and keyborad.
	ActionState(deltaTime);
	// Action State
	// -> The function that updating actions with attack and move

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
		unit->Render(mCamera->GetActiveCamera());
	for (auto& unit : mMutants)
		unit->Render(mCamera->GetActiveCamera());

	SimpleDraw::AddGroundPlane(80.0f, 2.0f, Colors::Black);
	SimpleDraw::Render(mCamera->GetActiveCamera());
}

void GameManager::DebugUI()
{
	mWorld.DebugUI();
	if (mUnit != nullptr)
	{
		JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(mUnit->GetPosition(), 1.5f), Colors::Aqua);
		if (UIManager::Get()->GetOrder() == UIManager::Order::Move && mUnit->GetAgent().GetSpeed() == 0)
			GridManager::Get()->GetGird().DisplayAreaCube(mUnit->GetAgent().mArea, mUnit->GetPosition(), GetColor(mCurrentState));

		if (UIManager::Get()->GetOrder() == UIManager::Order::Attack && !isAttacking)
			GridManager::Get()->GetGird().DisplayAreaCube(mUnit->GetAgent().mArea, mUnit->GetPosition(), Colors::Yellow);

		if (mUnit->GetStatus() != Status::TurnOver)
		{
			for (auto& unit : mSoldiers)
				JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(unit->GetPosition(), 1.0f), GetColor_Standby(unit->GetFlag()));
			for (auto& unit : mMutants)
				JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(unit->GetPosition(), 1.0f), GetColor_Standby(unit->GetFlag()));
		}
	}
	GridManager::Get()->GetGird().DebugUI();
}

void GameManager::Spawn(const JimmyGod::Math::Vector3& pos, const char* name, UnitType type, Flag flag)
{
	switch (type)
	{
	case UnitType::Soldier:
	{
		auto& newUnit = mSoldiers.emplace_back(new Soldier(name, flag));
		newUnit->Initialize(&mWorld);
		newUnit->GetAgent().GetTransformComponent().SetPosition(pos);
		newUnit->SetCoordinate(GridManager::Get()->GetGraph().GetNode(newUnit->GetPosition())->coordinate);
	} break;
	case UnitType::Mutant:
	{
		auto& newUnit = mMutants.emplace_back(new Mutant(name, flag));
		newUnit->Initialize(&mWorld);
		newUnit->GetAgent().GetTransformComponent().SetPosition(pos);
		newUnit->SetCoordinate(GridManager::Get()->GetGraph().GetNode(newUnit->GetPosition())->coordinate);
	} break;
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
		return Vector4();
		break;
	}

}

const JimmyGod::Graphics::Color GameManager::GetColor_Standby(Flag flag) const
{
	switch (flag)
	{
	case Flag::Ally:
		return Colors::ForestGreen;
		break;
	case Flag::Neutral:
		return Colors::Gold;
		break;
	case Flag::Enemy:
		return Colors::Purple;
		break;
	default:
		return Vector4();
		break;
	}
}

void GameManager::ActionState(float deltaTime)
{
	if (mUnit)
	{
		switch (mUnit->GetStatus())
		{
		case Status::Move:
		{
			if (mUnit->GetAgent().mPath.size() == 0 && mMoveActive)
			{
				mMoveActive = false;
				GridManager::Get()->GetGird().ObjectPosition(mUnit->GetPosition());
				mUnit->SetStatus(Status::TurnOver);
				mUnit = nullptr;
				mTarget = nullptr;

				UIManager::Get()->SetOrder(static_cast<uint32_t>(0));
			}
		}
		break;
		case Status::Attack:
		{
			if (mAttackActive)
			{
				mUnit->mTime = ATTACK_TIME;
				mAttackActive = false;
				mUnit->GetAgent().GetModelComponent().SetAnimationTime(0.0f);
				mUnit->SetProcess(true);
				isAttacking = true;
			}
			else
			{
				if (mUnit->AttackUpdate(deltaTime))
				{
					mUnit->SetProcess(false);
					UIManager::Get()->SetOrder(static_cast<uint32_t>(0));
					isAttacking = false;
					if (mTarget != nullptr) mUnit->Attack(*mTarget);
					mUnit->GetAgent().GetModelComponent().SetAnimationSpeed(1.0f);
					mTarget = nullptr;
					mUnit->SetStatus(Status::TurnOver);
					mUnit = nullptr;
				}
				else
				{
					auto direction = mUnit->GetPosition() - mTarget->GetPosition();
					mRotation = Quaternion::RotationLook(direction);
					mUnit->GetAgent().GetTransformComponent().SetRotation(Slerp(mUnit->GetAgent().GetTransformComponent().rot, mRotation, deltaTime * 10.0f));
				}
			}
		}
		break;
		default:
			break;
		}
	}
}

void GameManager::ControlState()
{
	auto& camera = mCamera->GetActiveCamera();
	bool check;
	auto inputSystem = InputSystem::Get();
	mMouseX = inputSystem->GetMouseScreenX();
	mMouseY = inputSystem->GetMouseScreenY();

	mRay = camera.ScreenPointToWorldRay(mMouseX, mMouseY);

	if (!mUnit)
	{
		if (inputSystem->IsMouseDown(MouseButton::LBUTTON) && !isAttacking)
		{
			mUnit = TraceUnit(mCurrentState);
			if (mUnit && mUnit->GetStatus() == Status::Standby)
			{
				GridManager::Get()->GetGird().ObjectPosition(mUnit->GetPosition());
				mUnit->SetCoordinate(GridManager::Get()->GetGraph().GetNode(mUnit->GetPosition())->coordinate);
				mDestination = mUnit->GetCoordinate();
				UIManager::Get()->ShowButtons();
			}
			else
				mUnit = nullptr;
		}
	}
	switch (UIManager::Get()->GetOrder())
	{
	case UIManager::Order::None:
		return;
		break;
	case UIManager::Order::Standby:
	{
		mUnit->SetStatus(Status::TurnOver);
		mUnit = nullptr;
		mTarget = nullptr;
		UIManager::Get()->SetOrder(static_cast<uint32_t>(0));
		UIManager::Get()->HideButtons();
	}
		break;
	case UIManager::Order::Move:
	{
		if (inputSystem->IsMouseDown(MouseButton::LBUTTON) && !mMoveActive)
		{
			for (auto& node : GridManager::Get()->GetGraph().GetNodes())
			{
				if (RayCast(mRay, node.collider, mMaxDistance))
				{
					check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(node.coordinate);
					if (check)
					{
						mDestination = node.coordinate;
						GridManager::Get()->GetGird().ObjectPosition(node.position);
						UIManager::Get()->HideButtons();

						mMoveActive = true;
						mUnit->SetStatus(Status::Move);
						mUnit->Move(mDestination);
						break;
					}
				}
			}
		}
	}
		break;
	case UIManager::Order::Attack:
	{
		if (inputSystem->IsMouseDown(MouseButton::LBUTTON) && !isAttacking)
		{
			bool check;
			if (mCurrentState != Flag::Neutral)
				mTarget = TraceUnit((mCurrentState == Flag::Ally ? Flag::Enemy : Flag::Ally));
			if (mTarget)
			{
				auto node = GridManager::Get()->GetGraph().GetNode(mTarget->GetPosition())->coordinate;
				check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(node);
				if (mTarget == mUnit || mTarget->GetFlag() == mUnit->GetFlag())
					mTarget = nullptr;
				if (check)
					GridManager::Get()->GetGird().ObjectPosition(mTarget->GetPosition());
				UIManager::Get()->HideButtons();

				mAttackActive = true;
				mUnit->SetStatus(Status::Attack);
			}
		}
	}
		break;
	default:
		break;
	}

	//if (inputSystem->IsMouseDown(MouseButton::LBUTTON) && !isAttacking)
	//{
	//	bool check;
	//	if (mCurrentState != Flag::Neutral)
	//		mTarget = TraceUnit((mCurrentState == Flag::Ally ? Flag::Enemy : Flag::Ally));
	//	if (mTarget)
	//	{
	//		auto node = GridManager::Get()->GetGraph().GetNode(mTarget->GetPosition())->coordinate;
	//		check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(node);
	//		if (mTarget == mUnit || mTarget->GetFlag() == mUnit->GetFlag())
	//			mTarget = nullptr;
	//		if (check)
	//			GridManager::Get()->GetGird().ObjectPosition(mTarget->GetPosition());
	//	}
	//	for (auto& node : GridManager::Get()->GetGraph().GetNodes())
	//	{
	//		if (RayCast(mRay, node.collider, mMaxDistance))
	//		{
	//			check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(node.coordinate);
	//			if (check)
	//			{
	//				mDestination = node.coordinate;
	//				GridManager::Get()->GetGird().ObjectPosition(node.position);
	//				break;
	//			}
	//		}
	//	}
	//}

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		if (!mAttackActive && !mMoveActive)
		{
			mUnit = nullptr;
			mTarget = nullptr;
			UIManager::Get()->HideButtons();
			UIManager::Get()->SetOrder(static_cast<uint32_t>(0));
		}
	}
}

Unit* GameManager::TraceUnit(Flag flag)
{
	for (auto& gameObject : mSoldiers)
		if (RayCast(mRay, gameObject->GetUnit(), mMaxDistance, flag) && gameObject->GetStatus() == Status::Standby)
			return &gameObject->GetUnit();

	for (auto& gameObject : mMutants)
		if (RayCast(mRay, gameObject->GetUnit(), mMaxDistance, flag) && gameObject->GetStatus() == Status::Standby)
			return &gameObject->GetUnit();

	return nullptr;
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
		if (unit->GetFlag() == mCurrentState && unit->GetStatus() != Status::TurnOver) return false;
	for (auto& unit : mMutants)
		if (unit->GetFlag() == mCurrentState && unit->GetStatus() != Status::TurnOver) return false;

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
	default: break;
	}

	for (auto& unit : mSoldiers)
		unit->SetStatus(Status::Standby);
	for (auto& unit : mMutants)
		unit->SetStatus(Status::Standby);
	return true;
}
