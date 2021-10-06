#include "GameManager.h"

#include "Flag.h"
#include "Unit.h"
#include "Soldier.h"
#include "Mutant.h"
#include "GridManager.h"
#include "UIManager.h"

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

	mCamera->AddCamera("Action");
	mCamera->AddCamera("Light");

	auto& camera = mCamera->GetActiveCamera();
	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(1750.0f);
	camera.SetPosition({ 0.0f, 70.0f, -1.0f });
	camera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mCamera->SetActiveCamera("Action");
	auto& actionCamera = mCamera->GetActiveCamera();
	actionCamera.SetNearPlane(0.1f);
	actionCamera.SetFarPlane(1750.0f);
	actionCamera.SetPosition({ 0.0f, 70.0f, -1.0f });
	actionCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mCamera->SetActiveCamera("Light");
	auto& lightCamera = mCamera->GetActiveCamera();
	lightCamera.SetNearPlane(0.1f);
	lightCamera.SetFarPlane(200.0f);
	lightCamera.SetPosition({ 0.0f, 70.0f, -1.0f });
	lightCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	lightCamera.SetDirection(Normalize({ 1.0f, -1.0f, 1.0f }));
	lightCamera.SetFov(1.0f);
	lightCamera.SetAspectRatio(1.0f);
	
	mCamera->SetActiveCamera("Default");
	mRender->SetDefaultCamera(mCamera->FindCamera("Default"));
	mRender->SetLightCamera(mCamera->FindCamera("Light"));

	mRender->Initialize();
}

void GameManager::Terminate()
{
	Reset();
	mRender->Terminate();
	mWorld.Terminate();
	for (auto& object : mBuildings)
	{
		object->Terminate();
		object.reset();
	}
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
	mBuildings.clear();
}

void GameManager::Update(float deltaTime)
{
	mWorld.Update(deltaTime);
	mRender->Update(deltaTime);
	
	ActionState(deltaTime); // -> The function that updating actions with attack and move
	for (auto& unit : mSoldiers)unit->Update(deltaTime);
	for (auto& unit : mMutants) unit->Update(deltaTime);
	for (auto& object : mBuildings) object->Update(deltaTime);

	TurnProcess(); // -> The function that investigate status for each unit.
	if (mCurrentState == Flag::Enemy)
		AIDecisionState(deltaTime);
	else
		ControlState(deltaTime); // -> The function that controls the unit with mouse and keyborad.
}

void GameManager::Render()
{
	auto matView = mCamera->GetActiveCamera().GetViewMatrix();
	auto matProj = mCamera->GetActiveCamera().GetPerspectiveMatrix();
	mWorld.Render();
	mRender->Render();
	for (auto& unit : mSoldiers) unit->Render(mCamera->GetActiveCamera());
	for (auto& unit : mMutants) unit->Render(mCamera->GetActiveCamera());
	for (auto& object : mBuildings) object->Render(mCamera->GetActiveCamera());
	SimpleDraw::AddGroundPlane(80.0f,-3.5f, 4.0f, Colors::Black);
	SimpleDraw::Render(mCamera->GetActiveCamera());
}

void GameManager::DebugUI()
{
	mWorld.DebugUI();

	if (mUnit != nullptr)
	{
		JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(mUnit->GetPosition(), 1.5f), Colors::Aqua);
		auto& order = UIManager::Get()->GetOrder();
		if (order == UIManager::Order::Move && mUnit->GetAgent().GetSpeed() == 0)
			GridManager::Get()->GetGird().DisplayAreaCube(mUnit->GetAgent().mArea, mUnit->GetPosition(), GetColor(mCurrentState));
		if (mUnit->GetFlag() == Flag::Enemy && mUnit->GetStatus() == Status::Standby)
		{
			GridManager::Get()->GetGird().DisplayAreaCube(mUnit->GetAgent().mArea, mUnit->GetPosition(), GetColor(mCurrentState));
			GridManager::Get()->GetGird().DisplayAreaCube(mUnit->GetAgent().mArea - 1, mUnit->GetPosition(), Colors::DarkRed);

			if (mUnit && mTarget)
				JimmyGod::Graphics::SimpleDraw::AddLine(Vector3{ mUnit->GetPosition().x,3.0f,mUnit->GetPosition().z}, 
					Vector3{ mTarget->GetPosition().x,3.0f,mTarget->GetPosition().z}, Colors::White);
		}
		if (order == UIManager::Order::Attack && mUnit->GetStatus() == Status::Standby)
			GridManager::Get()->GetGird().DisplayAreaCube(mUnit->GetAgent().mArea - 1, mUnit->GetPosition(), Colors::Yellow);
	}
	for (auto& unit : mSoldiers)
	{
		if (mCurrentState != unit->GetFlag()) continue; 
		if ((unit->GetStatus() == Status::TurnOver || unit->GetStatus() == Status::Dead)) continue;
		JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(unit->GetPosition(), 1.0f), GetColor_Standby(unit->GetFlag()));
	}

	for (auto& unit : mMutants)
	{
		if (mCurrentState != unit->GetFlag()) continue;
		if ((unit->GetStatus() == Status::TurnOver || unit->GetStatus() == Status::Dead)) continue;
		JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(unit->GetPosition(), 1.0f), GetColor_Standby(unit->GetFlag()));
	}

	GridManager::Get()->GetGird().DebugUI();
}

void GameManager::Spawn(const JimmyGod::Math::Vector3& pos, const char* name, UnitType type, Flag flag)
{
	switch (type)
	{
	case UnitType::Soldier:
	{
		auto& newUnit = mSoldiers.emplace_back((new Soldier(name, flag)));
		newUnit->Initialize(&mWorld);
		newUnit->GetAgent().GetTransformComponent()->SetPosition(pos);
		newUnit->SetCoordinate(GridManager::Get()->GetGird().GetGraph().GetNode(pos)->coordinate);
	} break;
	case UnitType::Mutant:
	{
		auto& newUnit = mMutants.emplace_back((new Mutant(name, flag)));
		newUnit->Initialize(&mWorld);
		newUnit->GetAgent().GetTransformComponent()->SetPosition(pos);
		newUnit->SetCoordinate(GridManager::Get()->GetGird().GetGraph().GetNode(pos)->coordinate);
	} break;
	default: return;
	}
}

void JimmyCom::GameManager::Spawn_Enviroment(const JimmyGod::Math::Vector3& pos, const char* name, bool destructible)
{
	if (name == "Building")
	{
		auto& newEnvironment = mBuildings.emplace_back(new Building(name + mEnvironmentIndex, destructible));
		newEnvironment->Initialize(&mWorld);
		newEnvironment->SetPosition(pos);
		newEnvironment->InstallGrid();
		mEnvironmentIndex++;
	}
}

Unit* JimmyCom::GameManager::TraceClosestUnit(Flag flag)
{
	Unit* unit = nullptr;
	float minDist = FLT_MAX;
	for (auto& gameObject : mSoldiers)
	{
		if (gameObject->GetStatus() == Status::Dead)
			continue;
		if (gameObject->GetFlag() == flag && gameObject->GetStatus() == Status::Standby)
		{
			const float dist = Distance(gameObject->GetPosition(), mUnit->GetPosition());
			if (minDist > dist)
			{
				minDist = dist;
				unit = &gameObject->GetUnit();
			}
		}
	}
	for (auto& gameObject : mMutants)
	{
		if (gameObject->GetStatus() == Status::Dead)
			continue;
		if (gameObject->GetFlag() == flag && gameObject->GetStatus() == Status::Standby)
		{
			const float dist = Distance(gameObject->GetPosition(), mUnit->GetPosition());
			if (minDist > dist)
			{
				minDist = dist;
				unit = &gameObject->GetUnit();
			}
		}
	}
	return unit;
}

Unit* JimmyCom::GameManager::TraceEnemy()
{
	for (auto& gameObject : mSoldiers)
	{
		if (gameObject->GetFlag() == Flag::Enemy && gameObject->GetStatus() == Status::Standby)
			return &gameObject->GetUnit();
	}
	for (auto& gameObject : mMutants)
	{
		if (gameObject->GetFlag() == Flag::Enemy && gameObject->GetStatus() == Status::Standby)
			return &gameObject->GetUnit();
	}
	return nullptr;
}

const JimmyGod::Graphics::Color GameManager::GetColor(Flag flag) const
{
	switch (flag)
	{
	case Flag::Ally: return Colors::Green;
	case Flag::Neutral: return Colors::Yellow;
	case Flag::Enemy: return Colors::Red;
	default: return Vector4();
	}
}

const JimmyGod::Graphics::Color GameManager::GetColor_Standby(Flag flag) const
{
	switch (flag)
	{
	case Flag::Ally: return Colors::LightGreen;
	case Flag::Neutral: return Colors::Gold;
	case Flag::Enemy: return Colors::Magenta;
	default: return Vector4();
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
			if (mUnit->GetAgent().mPath.size() == 0 && UIManager::Get()->GetOrder() == UIManager::Order::Move)
			{
				GridManager::Get()->GetGird().ObjectPosition(mUnit->GetPosition());
				mUnit->SetCoordinate(GridManager::Get()->GetGird().GetGraph().GetNode(mUnit->GetPosition())->coordinate);
				mUnit->SetStatus(Status::TurnOver);
				Reset();
			}
		} break;
		case Status::Attack:
		{
			if (mUnit->AttackUpdate(deltaTime))
			{
				mUnit->SetProcess(false);
				if (mTarget != nullptr) mUnit->Attack(*mTarget);
				mUnit->GetModelComponent().SetAnimationSpeed(1.0f);
				mUnit->SetStatus(Status::TurnOver);
				Reset();
				mTurnOver = ATTACK_TIME;
			}
			else
			{
				if(mCamera->GetActiveCamera().GetPosition().y > 40.0f)
					mCamera->GetActiveCamera().Walk(2.5f);

				mRotation = Quaternion::RotationLook(Normalize(mUnit->GetPosition() - mTarget->GetPosition()));
				mRotationTarget = Quaternion::RotationLook(Normalize(mTarget->GetPosition() - mUnit->GetPosition()));
				mUnit->GetAgent().GetTransformComponent()->SetRotation(Slerp(mUnit->GetAgent().GetTransformComponent()->GetRotation(), mRotation, deltaTime * 10.0f));
				mTarget->GetAgent().GetTransformComponent()->SetRotation(Slerp(mTarget->GetAgent().GetTransformComponent()->GetRotation(), mRotationTarget, deltaTime * 10.0f));

			}
		} break;
		default: break;
		}
	}
}

void GameManager::ControlState(float deltaTime)
{
	auto& camera = mCamera->GetActiveCamera();
	auto inputSystem = InputSystem::Get();
	bool check;
	mMouseX = inputSystem->GetMouseScreenX();
	mMouseY = inputSystem->GetMouseScreenY();
	mRay = camera.ScreenPointToWorldRay(mMouseX, mMouseY);

	if (!mUnit)
	{
		if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
		{
			mUnit = TraceUnit(mCurrentState);
			if (mUnit && mUnit->GetStatus() == Status::Standby)
			{
				GridManager::Get()->GetGird().ObjectPosition(mUnit->GetPosition());
				mUnit->SetCoordinate(GridManager::Get()->GetGird().GetGraph().GetNode(mUnit->GetPosition())->coordinate);
				mDestination = mUnit->GetCoordinate();
				UIManager::Get()->ShowButtons();
			}
			else mUnit = nullptr;
		}
	}
	switch (UIManager::Get()->GetOrder())
	{
	case UIManager::Order::None:
	{
		if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
			if (UIManager::Get()->GetOrder() == UIManager::Order::None) Reset();
		auto& camera = mCamera->GetActiveCamera();
		if (mCameraSwitched)
		{
			if (camera.GetPosition().y < 70.0f)
				camera.SetPosition(Lerp(camera.GetPosition(), Vector3{ 0.0f,70.0f,-1.0f }, deltaTime * 2.0f));
			else
				mCameraSwitched = false;
		}
		else
		{
			camera.SetPosition(Vector3{ 0.0f, 70.0f, -1.0f });
			camera.SetLookAt(Vector3{ 0.0f, 0.0f, 0.0f });
			mCamera->SetActiveCamera("Default");
		}
	}
		break;
	case UIManager::Order::Standby:
	{
		mUnit->SetStatus(Status::TurnOver);
		Reset();
	}
		break;
	case UIManager::Order::Move:
	{
		if (inputSystem->IsMouseDown(MouseButton::LBUTTON) && mUnit->GetStatus() != Status::Move)
		{
			for (auto& node : GridManager::Get()->GetGird().GetNodes())
			{
				if (RayCast(mRay, node.collider, mMaxDistance))
				{
					if (node.GetWalkable() == false)
						return;
					check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(node.coordinate);
					if (check)
					{
						mDestination = node.coordinate;
						GridManager::Get()->GetGird().ObjectPosition(node.position);
						UIManager::Get()->HideButtons();
						mUnit->SetStatus(Status::Move);
						mUnit->Move(mDestination);
						return;
					}
				}
			}
		}
		if (inputSystem->IsMouseDown(MouseButton::RBUTTON) && mUnit->GetStatus() != Status::Move) Reset();
	}
		break;
	case UIManager::Order::Attack:
	{
		if (inputSystem->IsMouseDown(MouseButton::LBUTTON) && mUnit->GetStatus() != Status::Attack)
		{
			bool check;
			if (mCurrentState != Flag::Neutral)
				mTarget = TraceUnit((mCurrentState == Flag::Ally ? Flag::Enemy : Flag::Ally));
			if (mTarget)
			{
				GridManager::Get()->GetGird().CalculateGrid(mUnit->GetAgent().mArea, mUnit->GetPosition());
				check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(mTarget->GetCoordinate());
				if (mTarget == mUnit || mTarget->GetFlag() == mUnit->GetFlag() || !check)
					mTarget = nullptr;

				if (mTarget != nullptr)
				{
					GridManager::Get()->GetGird().ObjectPosition(mTarget->GetPosition());
					UIManager::Get()->HideButtons();
					BeginAttack();
				}
			}
		}
		if (inputSystem->IsMouseDown(MouseButton::RBUTTON) && mUnit->GetStatus() != Status::Attack) Reset();
	}
		break;
	default: break;
	}
}

void JimmyCom::GameManager::AIDecisionState(float deltaTime)
{
	if (mUnit)
	{
		if (mUnit->GetAgent().mPath.empty() && mUnit->GetStatus() == Status::Move)
		{
			GridManager::Get()->GetGird().ObjectPosition(mUnit->GetPosition());
			mUnit->SetCoordinate(GridManager::Get()->GetGird().GetGraph().GetNode(mUnit->GetPosition())->coordinate);
			mUnit->SetStatus(Status::TurnOver);
			Reset();
			mProcessing_AI = false;
			mTurnOver += 1.5f;
		}
	}

	if (mProcessing_AI) return;

	if (mTurnOver > 0.0f)
	{
		mTurnOver -= deltaTime;
		auto& camera = mCamera->GetActiveCamera();
		if (mCameraSwitched)
		{
			if (camera.GetPosition().y < 70.0f)
				camera.SetPosition(Lerp(camera.GetPosition(), Vector3{ 0.0f,70.0f,-1.0f }, deltaTime * 2.0f));
			else
				mCameraSwitched = false;
		}
		else
		{
			camera.SetPosition(Vector3{ 0.0f, 70.0f, -1.0f });
			camera.SetLookAt(Vector3{ 0.0f, 0.0f, 0.0f });
			mCamera->SetActiveCamera("Default");
		}
		return;
	}

	if (!mUnit)
	{
		mUnit = TraceEnemy();
		mTarget = TraceClosestUnit(Flag::Ally);
		GridManager::Get()->GetGird().ObjectPosition(mUnit->GetPosition());
		mTurnOver += 2.0f;
		return;
	}

	if(mUnit && mUnit->GetFlag() == Flag::Enemy)
	{
		if (!mTarget)
			mUnit->SetStatus(Status::TurnOver);
		else
		{
			if (!mProcessing_AI)
			{
				GridManager::Get()->GetGird().CalculateGrid(mUnit->GetAgent().mArea - 1, mUnit->GetPosition());
				if (!GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(mTarget->GetCoordinate()))
				{
					auto pos = GridManager::Get()->GetGird().FindClosestPath(mUnit->GetAgent().mArea, mUnit->GetPosition(), mTarget->GetPosition());
					auto coord = GridManager::Get()->GetGird().GetGraph().GetNode(pos)->coordinate;
					mUnit->Move(coord);
					mUnit->SetStatus(Status::Move);
				}
				else
					BeginAttack();

				mProcessing_AI = true;
			}
		}
	}


}

void JimmyCom::GameManager::BeginAttack()
{
	mUnit->SetStatus(Status::Attack);
	mUnit->GetModelComponent().SetAnimationTime(0.0f);
	mUnit->mUpdateTime = ATTACK_TIME;
	mUnit->SetProcess(true); // For animation changing
	mCamera->SetActiveCamera("Action");
	mCamera->GetActiveCamera().SetPosition(Vector3{ mTarget->GetPosition().x,mTarget->GetPosition().y + 70.0f,mTarget->GetPosition().z });
	mCameraSwitched = true;
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

bool JimmyCom::GameManager::IsExist(const AI::Coord& coord) const
{
	for (auto& gameObject : mSoldiers)
		if (gameObject->GetCoordinate() == coord && gameObject->GetStatus() != Status::Dead) return true;
	for (auto& gameObject : mMutants)
		if (gameObject->GetCoordinate() == coord && gameObject->GetStatus() != Status::Dead) return true;
	return false;
}

bool GameManager::RayCast(const JimmyGod::Math::Ray& mousePoint, const Unit& collider, float& maxDistance, Flag layerMask)
{
	return ((layerMask == collider.GetFlag()) && Intersect(mousePoint, collider.GetSphereCollider(), maxDistance));
}

bool GameManager::RayCast(const JimmyGod::Math::Ray& mousePoint, const JimmyGod::Math::Sphere& collider, float& maxDistance)
{
	return Intersect(mousePoint, collider, maxDistance);
}

bool GameManager::TurnProcess()
{
	for (auto& unit : mSoldiers)
	{
		if (unit->GetStatus() == Status::Dead) continue;
		if (unit->GetFlag() == mCurrentState && unit->GetStatus() != Status::TurnOver) return false;
	}
		
	for (auto& unit : mMutants)
	{
		if (unit->GetStatus() == Status::Dead) continue;
		if (unit->GetFlag() == mCurrentState && unit->GetStatus() != Status::TurnOver) return false;
	}
		

	switch (mCurrentState)
	{
	case Flag::Ally: mCurrentState = Flag::Neutral; break;
	case Flag::Neutral: mCurrentState = Flag::Enemy; break;
	case Flag::Enemy: mCurrentState = Flag::Ally; break;
	default: break;
	}

	Reset();
	for (auto& unit : mSoldiers) 
		if(unit->GetStatus() != Status::Dead) unit->SetStatus(Status::Standby);
	for (auto& unit : mMutants)
		if (unit->GetStatus() != Status::Dead) unit->SetStatus(Status::Standby);
	mTurnOver += 1.5f;
	return true;
}

void JimmyCom::GameManager::Reset()
{
	mUnit = nullptr;
	mTarget = nullptr;
	UIManager::Get()->HideButtons();
	UIManager::Get()->SetOrder(static_cast<uint32_t>(0));
	mProcessing_AI = false;
}