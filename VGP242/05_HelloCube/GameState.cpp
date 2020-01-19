#include "GameState.h"

using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });



	mMesh.vertices.emplace_back(VertexPC{ Vector3{ -0.5f,0.5f,-0.5f }, Color{Colors::DeepSkyBlue} });
	mMesh.vertices.emplace_back(VertexPC{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Navy} });
	mMesh.vertices.emplace_back(VertexPC{ Vector3{ -0.5f,-0.5f,-0.5f }, Color{Colors::SkyBlue} });
	mMesh.vertices.emplace_back(VertexPC{ Vector3{ 0.5f,-0.5f,-0.5f}, Color{Colors::Aqua} });
	mMesh.vertices.emplace_back(VertexPC{ Vector3{ -0.5f,0.5f,0.5f }, Color{Colors::DeepSkyBlue} });
	mMesh.vertices.emplace_back(VertexPC{ Vector3{ 0.5f,0.5f,0.5f }, Color{Colors::Navy} });
	mMesh.vertices.emplace_back(VertexPC{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::Aqua} });
	mMesh.vertices.emplace_back(VertexPC{ Vector3{ 0.5f,-0.5f,0.5f }, Color{Colors::SkyBlue} });

	//Front
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(2);
	//Right
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(7);
	mMesh.indices.push_back(5);
	//Back
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(1);
	//Left
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(7);
	//Top
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(2);
	//Bottom
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(7);

	mMeshBuffer.Initialize(mMesh);

	mConstantBuffer.Initialize(sizeof(Matrix4));

	// Compile and create vertex shader
	mVertexShader.Initialize("../../Assets/Shaders/DoTransform.fx", VertexPC::Format);
	// Compile and create pixel shader
	mPixelShader.Initialize("../../Assets/Shaders/DoTransform.fx");
}

void GameState::Terminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
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
	mConstantBuffer.BindVS(0);

	for (int i = 0; i < 11; i++)
	{
		auto matWorld1 = Matrix4::RotationY(mRotation.y- static_cast<float>(i));
		auto matWorld2 = Matrix4::RotationX(mRotation.x- static_cast<float>(i));
		auto matWorld3 = Matrix4::RotationZ(mRotation.z+ static_cast<float>(i));
		auto matTranslation = Matrix4::Translation(Vector3(static_cast<float>(i), static_cast<float>(i), static_cast<float>(i)));
		auto matScl = Matrix4::Scaling(static_cast<float>(i) * 0.25f);
		auto matWVP = Transpose(matScl*matTranslation*matWorld1 * matWorld2 * matWorld3 * matView * matProj);
		
		mConstantBuffer.Update(&matWVP);
		mMeshBuffer.Draw();
	}
	/*context->Draw(mVertices.size(), 0);*/ // This is for when we don't have an index buffer
}