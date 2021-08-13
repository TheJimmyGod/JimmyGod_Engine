#include "GameState.h"
#include "Unit.h"
#include "CharacterModule.h"
#include "Soldier.h"
#include "Mutant.h"
#include "GridManager.h"
#include <iostream>
#include <ImGui/Inc/imgui.h>

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });

	mCamera = mWorld.AddService<CameraService>();
	mRender = mWorld.AddService<RenderService>();
	mWorld.Initialize(100);
	auto& camera = mCamera->GetActiveCamera();

	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(1750.0f);
	camera.SetPosition({ 0.0f, 70.0f, -1.0f });
	camera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mRender->Initialize();

	Johny = new Soldier("Johny", Flag::Ally);
	Johny->Initialize(&mWorld);

	Zone = new Mutant("Zone", Flag::Enemy);
	Zone->Initialize(&mWorld);

	GridManager::StaticInitialize(&mWorld);

	JimmyGod::Physics::PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f, -9.8f, 0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.3f;
	settings.iterations = 1;

	mPhysicsWorld.Initialize(settings);
	mPhysicsWorld.AddOBB(Johny->GetAgent().GetColliderComponent().GetOBB());
	mPhysicsWorld.AddOBB(Zone->GetAgent().GetColliderComponent().GetOBB());
	GridManager::Get()->GetGird().ObjectPosition(Johny->GetAgent().GetPosition());

	unit = Johny;
}

void GameState::Terminate()
{
	Johny->Terminate();
	Zone->Terminate();
	GridManager::StaticTerminate();
	
	delete Johny;
	Johny = nullptr;

	delete Zone;
	Zone = nullptr;

	mRender->Terminate();

	mWorld.Terminate();
	mPhysicsWorld.Clear();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	auto& camera = mCamera->GetActiveCamera();
	mRender->Update(deltaTime);
	fps = 1.0f / deltaTime;
	mWorld.Update(deltaTime);
	mPhysicsWorld.Update(deltaTime);

	float column = static_cast<float>(inputSystem->GetMouseScreenX());
	float row = static_cast<float>(inputSystem->GetMouseScreenY());
	Ray ray = camera.ScreenPointToWorldRay(column, row);

	Vector3 v = ray.GetPoint(80.0f);
	if (inputSystem->IsMousePressed(MouseButton::LBUTTON))
	{
		AI::Coord position = GridManager::Get()->GetCoordinate(Vector3{ v.x,0.0f,v.z });
		bool check = GridManager::Get()->GetGird().CheckMaximumAndMinimumGird(position);
		if (check)
		{
			destiniation = position;
			GridManager::Get()->GetGird().ObjectPosition(Vector3{ v.x,0.0f,v.z });
		}
	}

	if (inputSystem->IsKeyDown(KeyCode::SPACE))
	{
		isActive = true;
		switch (mTurn)
		{
		case Flag::Ally:
		{
			current = GridManager::Get()->GetGraph().GetCoordinate(Johny->GetAgent().GetPosition());
			GridManager::Get()->GetGird().FindPath(current, destiniation, Johny->GetAgent().mArea, Johny->GetAgent().mPath);
			if (Johny->GetAgent().mPath.size() > 0) Johny->GetAgent().ChangeState("Move");
		} break;
		case Flag::Neutral: break;
		case Flag::Enemy:
		{
			current = GridManager::Get()->GetGraph().GetCoordinate(Zone->GetAgent().GetPosition());
			GridManager::Get()->GetGird().FindPath(current, destiniation, Zone->GetAgent().mArea, Zone->GetAgent().mPath);
			if (Zone->GetAgent().mPath.size() > 0) Zone->GetAgent().ChangeState("Move");
		} break;
		default: break;
		}
	}

	if (unit != nullptr)
	{
		if (isActive == true && unit->GetAgent().mPath.size() == 0)
		{
			isActive = false;
			if (mTurn == Flag::Ally)
			{
				mTurn = Flag::Enemy;
				unit = Zone;
			}
			else
			{
				mTurn = Flag::Ally;
				unit = Johny;
			}
			GridManager::Get()->GetGird().ObjectPosition(unit->GetAgent().GetPosition());
		}
	}

	Johny->Update(deltaTime);
	Zone->Update(deltaTime);
}

void GameState::Render()
{
	auto matView = mCamera->GetActiveCamera().GetViewMatrix();
	auto matProj = mCamera->GetActiveCamera().GetPerspectiveMatrix();
	mWorld.Render();

	mRender->Render();

	Johny->Render(mCamera->GetActiveCamera());
	Zone->Render(mCamera->GetActiveCamera());

	SimpleDraw::AddGroundPlane(80.0f, 2.0f, Colors::Black);
	SimpleDraw::Render(mCamera->GetActiveCamera());
}

void GameState::DebugUI()
{
	mWorld.DebugUI();
	static bool OBBcollision = false;

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("fps: %.2f", fps);
		if (ImGui::CollapsingHeader("Debug UI Option", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Checkbox("OBB Debug UI active/inactive", &OBBcollision))
			{
				Johny->GetAgent().GetColliderComponent().Active(OBBcollision);
				Zone->GetAgent().GetColliderComponent().Active(OBBcollision);
			}
		}
	}
	if (unit != nullptr)
	{
		if (unit->GetAgent().GetSpeed() == 0)
			GridManager::Get()->GetGird().DisplayAreaCube(
				unit->GetAgent().mArea,
				unit->GetAgent().GetPosition());
	}
	GridManager::Get()->GetGird().DebugUI();
	ImGui::End();
}