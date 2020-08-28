#include "GameState.h"

#include <ImGui/Inc/imgui.h>

using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(500.0f);
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

	mWorld.Initialize(100);
	mWorld.Create("../../Assets/Templates/tallBox.json", "Jimmy");
	mWorld.Create("../../Assets/Templates/longBox.json", "Jimmy");
	mWorld.Create("../../Assets/Templates/fatBox.json", "Jimmy");
	//mWorld.Create("tallBox", "Jimmy");
	//mWorld.Create("longBox", "Jimmy");
	//mWorld.Create("fatBox", "Jimmy");
}

void GameState::Terminate()
{
	mWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;

	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	mWorld.Update(deltaTime);
	
}

void GameState::Render()
{
	mWorld.Render();


	SimpleDraw::AddGroundPlane(30.0f,Colors::Aqua);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	mWorld.DebugUI();
}