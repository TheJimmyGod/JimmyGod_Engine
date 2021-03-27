#include "GameState.h"

#include <ImGui/Inc/imgui.h>

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera = mWorld.AddService<CameraService>();
	mWorld.Initialize(100);

	auto& camera = mCamera->GetActiveCamera();

	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(500.0f);
	camera.SetPosition({ 0.0f, 1.0f, -5.0f });
	camera.SetLookAt({ 0.0f, 1.0f, 0.0f });

	//mWorld.Create("../../Assets/Templates/Test.json", "Jimmy");
	mWorld.Create("../../Assets/Templates/tallBox.json", "Jimmy");
	mWorld.Create("../../Assets/Templates/longBox.json", "Jimmy");
	mWorld.Create("../../Assets/Templates/fatBox.json", "Jimmy");

	// mWorld.Create("../../Assets/Templates/FPSCamera.json", "Jimmy");
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

	auto& camera = mCamera->GetActiveCamera();

	if (inputSystem->IsKeyDown(KeyCode::W))
		camera.Walk(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		camera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		camera.Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		camera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		camera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	mWorld.Update(deltaTime);
	
}

void GameState::Render()
{
	mWorld.Render();

	SimpleDraw::AddGroundPlane(30.0f,Colors::Aqua);
	SimpleDraw::Render(mCamera->GetActiveCamera());
}

void GameState::DebugUI()
{
	mWorld.DebugUI();
}