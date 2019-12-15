#include "GameState.h"

using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	/* Homework
	 Update helloTexturing to use a MeshPX data to draw a texture mapped cubes
	 You will need to add Sampler and Texture classes provided
	 You will need to use DoTexturing fx shaders
	 Add a new class called Graphics::MeshBuilder with the following functions
	
	 namespace JimmyGod::Graphics
	 {
			class MeshBuilder
			{
	static MeshPX CreatePlanePX();
	static MeshPX CreateCylinderPX();
	static MeshPX CreateSpherePX(float radius, int ringhs = 16, int slices = 16);
			}
	This will allow you to create a mesh easily by doing:

	auto mesh = MeshBuilder::CreateSpherePX(...);

	Add HelloEarth to test a texture mapped sphere using Earth texture

	a plane:
	for(int y=0; y< height; ++y)
		for(int x=0; x<width; ++x)
			mMesh.push_back({x,y,0.0f}...);

	a cylinder:
	for(int y=0; y< height; ++y)
		for(int theta=0; theta<TwoPi; theta += ...)
			mMesh.push_back({sin(theta),y,cos(theta)}...);

	a sphere:
	for(int phi=0; phi< Pi; phi += ...)
		for(int theta=0; theta<TwoPi; theta += ...)
			mMesh.push_back({sin(theta) * r,phi,cos(theta) *r}...);

	 }*/

	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,-1.0f}, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,-1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,-1.0f}, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,-1.0f}, 1.0f,1.0f });

	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,-1.0f }, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,-1.0f }, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,1.0f}, 1.0f,1.0f });

	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,1.0f }, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,1.0f }, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,1.0f}, 1.0f,1.0f });

	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,1.0f}, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,-1.0f }, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,-1.0f}, 1.0f,1.0f });

	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,1.0f}, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,1.0f }, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,1.0f,-1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,1.0f,-1.0f}, 1.0f,1.0f });

	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,-1.0f}, 0.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,-1.0f }, 0.0f,1.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ 1.0f,-1.0f,1.0f }, 1.0f,0.0f });
	mMesh.vertices.emplace_back(VertexPX{ Vector3{ -1.0f,-1.0f,1.0f}, 1.0f,1.0f });

	//Front
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(2);
	//Right
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(7);
	mMesh.indices.push_back(6);
	//Back
	mMesh.indices.push_back(8);
	mMesh.indices.push_back(9);
	mMesh.indices.push_back(10);
	mMesh.indices.push_back(9);
	mMesh.indices.push_back(11);
	mMesh.indices.push_back(10);
	//Left
	mMesh.indices.push_back(12);
	mMesh.indices.push_back(13);
	mMesh.indices.push_back(14);
	mMesh.indices.push_back(13);
	mMesh.indices.push_back(15);
	mMesh.indices.push_back(14);
	//Top
	mMesh.indices.push_back(16);
	mMesh.indices.push_back(17);
	mMesh.indices.push_back(18);
	mMesh.indices.push_back(17);
	mMesh.indices.push_back(19);
	mMesh.indices.push_back(18);
	//Bottom
	mMesh.indices.push_back(21);
	mMesh.indices.push_back(20);
	mMesh.indices.push_back(22);
	mMesh.indices.push_back(21);
	mMesh.indices.push_back(22);
	mMesh.indices.push_back(23);

	mMeshBuffer.Initialize(mMesh);	

	mConstantBuffer.Initialize(sizeof(Matrix4));
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Clamp);
	mTexture.Initialize("../../Assets/Textures/SimYoung.jpg");
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

	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mRotation += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mRotation -= deltaTime;
	}
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
	for (int i = 0; i < 3; i++)
	{
		auto matWorld1 = Matrix4::RotationY(mRotation.y);
		//auto matWorld2 = Matrix4::RotationX(mRotation.x- static_cast<float>(i));
		//auto matWorld3 = Matrix4::RotationZ(mRotation.z+ static_cast<float>(i));
		auto matTranslation = Matrix4::Translation(Vector3(static_cast<float>(i), static_cast<float>(i), static_cast<float>(i)));
		auto matScl = Matrix4::Scaling(static_cast<float>(i) * 0.25f);
		auto matWVP = Transpose(matScl*matTranslation*matWorld1 * matView * matProj);
		
		mConstantBuffer.Set(&matWVP);
		mMeshBuffer.Draw();
	}
	/*context->Draw(mVertices.size(), 0);*/ // This is for when we don't have an index buffer
}