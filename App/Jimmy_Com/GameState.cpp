#include "GameState.h"
#include "Unit.h"
#include "CharacterModule.h"
#include "Soldier.h"
#include "Mutant.h"
#include "GridManager.h"
#include "GameManager.h"
#include "UIManager.h"
#include <iostream>
#include <ImGui/Inc/imgui.h>

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;
using namespace JimmyCom;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	GameManager::StaticInitialize(100);
	GridManager::StaticInitialize(&GameManager::Get()->GetGameWorld());

	GameManager::Get()->Spawn_Enviroment({ -12.5f,0.0f,-8.5f },"Building",false);
	GameManager::Get()->Spawn_Enviroment({ 8.5f,0.0f,-8.5f }, "Building", false);
	GameManager::Get()->Spawn({ 0.0f,0.0f,14.0f }, "Jimmy", UnitType::Soldier, Flag::Ally);
	GameManager::Get()->Spawn({ 12.0f,0.0f,14.0f }, "Sushi", UnitType::Soldier, Flag::Ally);
	GameManager::Get()->Spawn({ 0.0f,0.0f,10.0f }, "Robort", UnitType::Soldier, Flag::Enemy);
	GameManager::Get()->Spawn({ 0.0f,0.0f,-20.0f }, "God", UnitType::Mutant, Flag::Enemy);

	JimmyGod::Physics::PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f, -9.8f, 0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.3f;
	settings.iterations = 1;

	mPhysicsWorld.Initialize(settings);
	UIManager::StaticInitialize();
}

void GameState::Terminate()
{
	GameManager::StaticTerminate();
	UIManager::StaticTerminate();
	mSpark.Terminate();
	mPhysicsWorld.Clear();
	GridManager::StaticTerminate();
}

void GameState::Update(float deltaTime)
{
	fps = 1.0f / deltaTime;
	GameManager::Get()->Update(deltaTime);

	UIManager::Get()->Update(deltaTime);
	auto unit = (GameManager::Get()->SelectedUnit() == nullptr ? nullptr : GameManager::Get()->SelectedUnit());
	auto target = (GameManager::Get()->SelectedTarget() == nullptr ? nullptr : GameManager::Get()->SelectedTarget());
	UIManager::Get()->UpdateAnimation(unit, target, 1.6f);
	mPhysicsWorld.Update(deltaTime);
}

void GameState::Render()
{
	GameManager::Get()->Render();
	UIManager::Get()->Render(GameManager::Get()->GetGameWorld().GetService<CameraService>()->GetActiveCamera());
}

void GameState::DebugUI()
{
	GameManager::Get()->DebugUI();
	static bool OBBcollision = false;

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("fps: %.2f", fps);
		if (ImGui::CollapsingHeader("Debug UI Option", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Checkbox("OBB Debug UI active/inactive", &OBBcollision))
			{

			}
		}
	}
	ImGui::End();
}