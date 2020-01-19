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
	mMesh = MeshBuilder::CreateSpherePN(15);
	mMeshBuffer.Initialize(mMesh);
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();

	mMaterialBuffer.Initialize();

	mVertexShader.Initialize("../../Assets/Shaders/DoPhongLighting.fx", VertexPN::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoPhongLighting.fx");
}

void GameState::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mMeshBuffer.Terminate();
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

	//SimpleDraw::AddDisplacement(25.0f);
	SimpleDraw::AddBox(15, Colors::Aqua);
	//SimpleDraw::AddSphere(20, 12, 720, Colors::White);

	mRotation += deltaTime;
}

void GameState::Render()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	auto matTrans = Matrix4::Translation({ 0.0f });
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	auto matTranslation = Matrix4::Identity;
	auto matWorld = matRot * matTrans;
	auto matWVP = Transpose(matTranslation*matWorld * matView * matProj);
	TransformData transformData;
	transformData.world = Transpose(matRot);
	transformData.wvp = Transpose(matWorld);

	mTransformBuffer.Update(&matWVP);
	mTransformBuffer.BindVS(0);

	mLightBuffer.Update(&matWVP);
	mLightBuffer.BindVS(1);
	
	mMaterialBuffer.Update(&matWVP);
	mMaterialBuffer.BindVS(2);

	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Draw();
	
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::ShowDemoWindow();
}
