#include "GameState.h"
#include "Unit.h"
#include "CharacterModule.h"
#include "Soldier.h"
#include "Mutant.h"
#include "GridManager.h"
#include "GameManager.h"
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
	GameManager::StaticInitialize(100);
	GridManager::StaticInitialize(&GameManager::Get()->GetGameWorld());;

	GameManager::Get()->Spawn({ 0.0f,0.0f,14.0f }, "Jimmy", UnitType::Soldier, Flag::Ally);
	GameManager::Get()->Spawn({ 12.0f,0.0f,14.0f }, "Sushi", UnitType::Soldier, Flag::Ally);
	GameManager::Get()->Spawn({ 0.0f,0.0f,10.0f }, "Robort", UnitType::Soldier, Flag::Ally);
	GameManager::Get()->Spawn({ 0.0f,0.0f,-20.0f }, "God", UnitType::Mutant, Flag::Enemy);

	JimmyGod::Physics::PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f, -9.8f, 0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.3f;
	settings.iterations = 1;

	mPhysicsWorld.Initialize(settings);
	mSpark.Initialize("../../Assets/Textures/Neptune.jpg", 40, 0.2f);
}

void GameState::Terminate()
{
	GameManager::StaticTerminate();
	GridManager::StaticTerminate();

	mSpark.Terminate();
	mPhysicsWorld.Clear();
}

void GameState::Update(float deltaTime)
{
	fps = 1.0f / deltaTime;
	GameManager::Get()->Update(deltaTime);
	mSpark.Update(deltaTime);
	if (GameManager::Get()->SelectedUnit() && GameManager::Get()->SelectedTarget())
	{
		auto unit = GameManager::Get()->SelectedUnit();
		auto target = GameManager::Get()->SelectedTarget();
		float time = unit->mTime;
		if ((time >= 1.5f && time < 1.6f) && !mAttack)
		{
			mAttack = true;
			if (unit->GetUnitType() == UnitType::Soldier)
			{
				unit->GetAgent().GetModelComponent().SetAnimationSpeed(1.1f);
				unit->GetAgent().GetModelComponent().PlayAnimation(4);
				unit->GetAgent().GetModelComponent().SetAnimationTime(0.0f);
			}
			else
			{
				unit->GetAgent().GetModelComponent().SetAnimationSpeed(1.1f);
				unit->GetAgent().GetModelComponent().PlayAnimation(2);
				unit->GetAgent().GetModelComponent().SetAnimationTime(0.0f);
			}

		}
		else if (time >= 0.5f && time < 0.6f && !SparkEffect)
		{
			SparkEffect = true;
			Bone* toe = nullptr;
			if (unit->GetUnitType() == UnitType::Soldier)
				toe = FindBone(unit->GetAgent().GetModelComponent().GetModel().mSkeleton, "mixamorig1:RightFoot");
			else
				toe = FindBone(unit->GetAgent().GetModelComponent().GetModel().mSkeleton, "Mutant:RightHandPinky3");
			mSpark.ShowSpark(unit->GetAgent().GetPosition() + GetTranslation(unit->GetAgent().GetModelComponent().GetBoneMatrices()[toe->index]
				* Matrix4::RotationQuaternion(unit->GetAgent().GetTransformComponent().GetRotation())) * 0.04f, target->GetAgent().GetPosition() - unit->GetAgent().GetPosition(), 2.2f);
		}
	}
	else
	{
		mAttack = false;
		SparkEffect = false;
	}


	mPhysicsWorld.Update(deltaTime);
}

void GameState::Render()
{
	GameManager::Get()->Render();
	mSpark.Render(GameManager::Get()->GetGameWorld().GetService<CameraService>()->GetActiveCamera());
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