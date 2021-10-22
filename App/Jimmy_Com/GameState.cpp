#include "GameState.h"
#include "Unit.h"
#include "CharacterModule.h"
#include "Soldier.h"
#include "Mutant.h"
#include "GridManager.h"
#include "GameManager.h"
#include "UIManager.h"
#include "HUD.h"
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
	UIManager::StaticInitialize();
}

void GameState::Terminate()
{
	mTerrain.Terminate();
	GameManager::StaticTerminate();
	UIManager::StaticTerminate();
	mSpark.Terminate();
	mPhysicsWorld.Clear();
	GridManager::StaticTerminate();
}

void GameState::Update(float deltaTime)
{
	UIManager::Get()->Update(deltaTime);
	if (mInitialize == false)
	{
		if (HUD::Get() == nullptr)
			return;
		if (HUD::Get()->GetState() == HUD::State::GamePlay)
		{
			Setup();
		}
		else if (HUD::Get()->GetState() == HUD::State::End)
		{
			SpriteRenderManager::Get()->DrawScreenText("Press Escape Key to end", 400.0f, 300.0f, 50.0f, Colors::White);
		}
		return;
	}
	fps = 1.0f / deltaTime;
	GameManager::Get()->Update(deltaTime);


	auto unit = (GameManager::Get()->SelectedUnit() == nullptr ? nullptr : GameManager::Get()->SelectedUnit());
	auto target = (GameManager::Get()->SelectedTarget() == nullptr ? nullptr : GameManager::Get()->SelectedTarget());
	UIManager::Get()->UpdateAnimation(unit, target, 1.6f);
	mPhysicsWorld.Update(deltaTime);
}

void GameState::Render()
{
	UIManager::Get()->HUD_Render();
	if (mInitialize == false)
	{
		HUD::Get()->DisplayMainMenu();
		return;
	}
	GameManager::Get()->Render();
	UIManager::Get()->Render(GameManager::Get()->GetGameWorld().GetService<CameraService>()->GetActiveCamera());

	auto matView = GameManager::Get()->GetGameWorld().GetService<CameraService>()->GetActiveCamera().GetViewMatrix();
	auto matProj = GameManager::Get()->GetGameWorld().GetService<CameraService>()->GetActiveCamera().GetPerspectiveMatrix();
	mTerrain.Render(GameManager::Get()->GetGameWorld().GetService<CameraService>()->GetActiveCamera());
}

void GameState::DebugUI()
{
	if (mInitialize == false)
		return;
	UIManager::Get()->DebugUI();
	GameManager::Get()->DebugUI();
	static bool Debug = false;

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("fps: %.2f", fps);
		if (ImGui::CollapsingHeader("Debug UI Option", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Checkbox("Skeleton Debug UI active/inactive", &Debug))
			{
				for (size_t i = 0; i < GameManager::Get()->GetMutantCount(); i++)
				{
					auto unit = GameManager::Get()->GetMutant(i).get();
					unit->GetAgent().GetOwner().GetComponent<ModelComponent>()->EnableDebug();
				}
				for (size_t i = 0; i < GameManager::Get()->GetSoldierCount(); i++)
				{
					auto unit = GameManager::Get()->GetSoldier(i).get();
					unit->GetAgent().GetOwner().GetComponent<ModelComponent>()->EnableDebug();
				}
				UIManager::Get()->EnableDebugUI();
			}
		}
	}
	ImGui::End();
}

void GameState::Setup()
{
	mTerrain.Initialize(100, 100, 1.0f);
	mTerrain.SetHeightScale(1.0f);

	GameManager::StaticInitialize(100);
	GridManager::StaticInitialize(&GameManager::Get()->GetGameWorld());

	GameManager::Get()->Spawn_Enviroment({ -12.5f,0.0f,-8.5f }, "Building", false);
	GameManager::Get()->Spawn_Enviroment({ 8.5f,0.0f,5.5f }, "Building", false);
	GameManager::Get()->Spawn_Enviroment({ 8.0f,0.0f,-4.0f }, "Grass", false);

	GameManager::Get()->Spawn_Enviroment({ 17.5f,0.0f,-12.0f }, "Building", false);
	GameManager::Get()->Spawn_Enviroment({ -15.0f,0.0f,0.5f }, "Grass", false);
	GameManager::Get()->Spawn_Enviroment({ 11.5f,0.0f,22.5f }, "Building", false);


	GameManager::Get()->Spawn({ 0.0f,0.0f,14.0f }, "Jimmy", UnitType::Soldier, Flag::Ally);
	GameManager::Get()->Spawn({ 12.0f,0.0f,14.0f }, "Sushi", UnitType::Soldier, Flag::Ally);
	GameManager::Get()->Spawn({ -10.0f,0.0f,-25.0f }, "Robort", UnitType::Mutant, Flag::Enemy);
	GameManager::Get()->Spawn({ 0.0f,0.0f,-20.0f }, "God", UnitType::Mutant, Flag::Enemy);

	JimmyGod::Physics::PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f, -9.8f, 0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.3f;
	settings.iterations = 1;

	mPhysicsWorld.Initialize(settings);

	mInitialize = true;
}
