#include "GameState.h"

using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

//		   +---------------+
//		  /				  /|
//		 /				 / |
// 1.0	+---------------+  |
//		|		^		|  |
//		|		|		|  |
//		|<------|------>|  |1.0
//		|		|		| /
//		|		v		|/
//		+---------------+ -1.0
//	 -1.0			   1.0

	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,-0.5f }, Color{Colors::DeepSkyBlue} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Navy} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,-0.5f }, Color{Colors::SkyBlue} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,-0.5f}, Color{Colors::Aqua} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,0.5f }, Color{Colors::DeepSkyBlue} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,0.5f }, Color{Colors::Navy} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::Aqua} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,0.5f }, Color{Colors::SkyBlue} });
	Vertex* arr = mMesh.mVertices.data();
	
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(1);
	mMesh.mIndices.push_back(2);
	mMesh.mIndices.push_back(1);
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(2);
	mMesh.mIndices.push_back(4);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(7);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(4);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(1);
	mMesh.mIndices.push_back(2);
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(7);
	mMesh.mIndices.push_back(4);
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(2);
	mMesh.mIndices.push_back(1);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(7);


	uint32_t* index = mMesh.mIndices.data();

	auto vertexCount = mMesh.mVertices.size() + 1;
	indiceCount = mMesh.mIndices.size();

	////Front
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,-0.5f }, Color{Colors::DeepSkyBlue} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Navy} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,-0.5f }, Color{Colors::SkyBlue} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,-0.5f }, Color{Colors::DeepSkyBlue} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Navy} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f, -0.5f,-0.5f }, Color{Colors::SkyBlue} });
	////left
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f, -0.5f }, Color{Colors::PapayaWhip} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::PapayaWhip} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f, 0.5f }, Color{Colors::BurlyWood} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f, 0.5f }, Color{Colors::PapayaWhip} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f, -0.5f }, Color{Colors::PapayaWhip} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f, -0.5f }, Color{Colors::BurlyWood} });
	////Back
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::Crimson} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,0.5f }, Color{Colors::Crimson} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,0.5f }, Color{Colors::Tan} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f, -0.5f,0.5f }, Color{Colors::Tan} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,0.5f }, Color{Colors::Crimson} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::Crimson} });
	////Right
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Yellow} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,0.5f }, Color{Colors::Yellow} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,-0.5f }, Color{Colors::GreenYellow} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f, -0.5f }, Color{Colors::Yellow} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f, 0.5f }, Color{Colors::GreenYellow} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f, 0.5f }, Color{Colors::Yellow} });
	////Top
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Chocolate} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,-0.5f }, Color{Colors::SandyBrown} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f, 0.5f }, Color{Colors::Chocolate} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,0.5f }, Color{Colors::SandyBrown} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,0.5f }, Color{Colors::Chocolate} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f, -0.5f }, Color{Colors::Chocolate} });
	////Bottom
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,-0.5f }, Color{Colors::Purple} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,-0.5f }, Color{Colors::Red} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f, 0.5f }, Color{Colors::Blue} });
	//mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,0.5f }, Color{Colors::Blue} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::Red} });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f, -0.5f }, Color{Colors::Purple} });

	mMeshBuffer.Initialize(arr, vertexCount, index , indiceCount);

	// Compile and create vertex shader
	mVertexShader.Initialize();
	// Compile and create pixel shader
	mPixelShader.Initialize();

	mConstantBuffer.Initialize(sizeof(Matrix4) * 3.0f);
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
	for (int i = 0; i < 100; i++)
	{
		auto matWorld1 = Matrix4::RotationY(mRotation+ i);
		auto matWorld2 = Matrix4::RotationX(mRotation-i);
		auto matWorld3 = Matrix4::RotationZ(mRotation);
		auto matView = mCamera.GetViewMatrix();
		auto matProj = mCamera.GetPerspectiveMatrix();
		auto matTranslation = Matrix4::Translation(Vector3((float)i, (float)i, (float)i));
		auto matScl = Matrix4::Scaling((float) i * 0.25f);
		auto matWVP = Transpose(matScl*matTranslation*matWorld1 * matWorld2 * matWorld3 * matView * matProj * (float)i);
		
		mVertexShader.Bind();
		mPixelShader.Bind();
		mMeshBuffer.Draw();
		mConstantBuffer.Set(&matWVP);
		mConstantBuffer.Bind();
	}
	/*context->Draw(mVertices.size(), 0);*/ // This is for when we don't have an index buffer

}