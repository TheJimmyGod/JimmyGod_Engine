#include "GameState.h"
#include <ImGui/Inc/imgui.h>
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ -75.0f,0.0f,-80.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	//SimpleDraw::StaticInitialize(1000000);
}

void GameState::Terminate()
{
	//SimpleDraw::StaticTerminate();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 100.5f;
	const float kTurnSpeed = 0.5f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(kMoveSpeed*deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-kMoveSpeed * deltaTime);
	}
	mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
	mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);

	SimpleDraw::AddBox(3, Colors::Aqua);

	mRotation += deltaTime;
}

void GameState::Render()
{
	//auto matView = mCamera.GetViewMatrix();
	//auto matProj = mCamera.GetPerspectiveMatrix();

	//mVertexShader.Bind();
	//mPixelShader.Bind();
	//mConstantBuffer.Bind();
	//mSampler.Bind();
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::ShowDemoWindow();
}
