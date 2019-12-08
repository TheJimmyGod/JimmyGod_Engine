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


	//Vertex verteies[] =
	//{
	//	//Front
	//	{ Vector3{ -0.5f,0.5f,-0.5f }, Color{Colors::DeepSkyBlue}},
	//	{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Navy} },
	//	{ Vector3{ -0.5f,-0.5f,-0.5f }, Color{Colors::SkyBlue} },
	//	{ Vector3{	0.5f,-0.5f,-0.5f}, Color{Colors::Aqua} },
	//	//Back
	//	{ Vector3{ -0.5f,0.5f,0.5f }, Color{Colors::DeepSkyBlue} },
	//	{ Vector3{ 0.5f,0.5f,0.5f }, Color{Colors::Navy} },
	//	{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::SkyBlue} },
	//	{ Vector3{	0.5f,-0.5f,0.5f}, Color{Colors::Aqua} }
	//};

	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,-0.5f }, Color{Colors::DeepSkyBlue} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Navy} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,-0.5f }, Color{Colors::SkyBlue} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,-0.5f}, Color{Colors::Aqua} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,0.5f }, Color{Colors::DeepSkyBlue} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,0.5f }, Color{Colors::Navy} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::Aqua} });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,0.5f }, Color{Colors::SkyBlue} });
	Vertex* arr = mMesh.mVertices.data();
	uint32_t indices[] =
	{
		0,1,2,
		1,3,2,
		
		4,6,5,
		6,7,5,

		4,5,8,
		0,5,1,

		2,3,6,
		6,3,7,
		
		4,0,6,
		6,0,2,

		1,5,3,
		3,5,7
	};
	
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

	mConstantBuffer.Initialize(sizeof(Matrix4));
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
	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	auto matWVP = Transpose(matWorld * matView * matProj);
	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Draw(indiceCount);
	mConstantBuffer.Set(&matWVP);
	mConstantBuffer.Bind();
	/*context->Draw(mVertices.size(), 0);*/ // This is for when we don't have an index buffer

}