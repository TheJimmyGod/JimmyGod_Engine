#include "GameState.h"

using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	auto Sphere = MeshBuilder::CreateSpherePX(20);
	mMeshBuffer.Initialize(Sphere);	

	mConstantBuffer.Initialize(sizeof(Matrix4));
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Clamp);
	mTexture.Initialize("../../Assets/Textures/Earth.jpg");
	// Compile and create vertex shader
	mVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx",VertexPX::Format);
	// Compile and create pixel shader
	mPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");

}

void GameState::Terminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 7.5f;
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

	mRotation += deltaTime;
}

void GameState::Render()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mConstantBuffer.Bind();
	mSampler.Bind();
	mTexture.Bind();
	for (int i = 0; i < 2; i++)
	{
		auto matWorld1 = Matrix4::RotationY(mRotation.y);
		auto matTranslation = Matrix4::Translation(Vector3(static_cast<float>(i), static_cast<float>(i), static_cast<float>(i)));
		auto matScl = Matrix4::Scaling(static_cast<float>(i) * 0.25f);
		auto matWVP = Transpose(matScl*matTranslation*matWorld1 * matView * matProj);
		
		mConstantBuffer.Set(&matWVP);
		mMeshBuffer.Draw();
	}
	/*context->Draw(mVertices.size(), 0);*/ // This is for when we don't have an index buffer
}