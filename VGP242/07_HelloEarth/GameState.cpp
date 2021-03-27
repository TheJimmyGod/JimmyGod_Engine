#include "GameState.h"

using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ -75.0f,0.0f,-80.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	auto Sphere = MeshBuilder::CreateSpherePX(20,12,36,false);
	mMeshBuffer.Initialize(Sphere);	

	mConstantBuffer.Initialize(sizeof(Matrix4));
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Clamp);
	mSun.Initialize("../../Assets/Textures/Sun.jpg");
	mEarth.Initialize("../../Assets/Textures/Earth.jpg");
	mMoon.Initialize("../../Assets/Textures/Moon.jpg");
	mVenus.Initialize("../../Assets/Textures/Venus.jpg");
	mMercury.Initialize("../../Assets/Textures/Mercury.jpg");
	mMars.Initialize("../../Assets/Textures/Mars.jpg");
	mJupiter.Initialize("../../Assets/Textures/Jupiter.jpg");
	mSaturn.Initialize("../../Assets/Textures/Saturn.jpg");
	mUranos.Initialize("../../Assets/Textures/Uranos.jpg");
	mNeptune.Initialize("../../Assets/Textures/Neptune.jpg");

	auto Space = MeshBuilder::CreateSpherePX(1000, 12, 360, true);
	mMeshDomeBuffer.Initialize(Space);
	mSpace.Initialize("../../Assets/Textures/Space.jpg");
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
	mMeshDomeBuffer.Terminate();
	mConstantBuffer.Terminate();
	mEarth.Terminate();
	mMoon.Terminate();
	mSun.Terminate();
	mMars.Terminate();
	mVenus.Terminate();
	mMercury.Terminate();
	mJupiter.Terminate();
	mSaturn.Terminate();
	mUranos.Terminate();
	mNeptune.Terminate();
	mSampler.Terminate();
	mSpace.Terminate();
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

	mRotation += deltaTime;
}

void GameState::Render()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mConstantBuffer.BindPS();
	mSampler.BindPS();

	auto matWorld0 = Matrix4::RotationY(mRotation.y * 0.25f);
	auto matTranslation0 = Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f));
	auto matSpace = matTranslation0 * matWorld0;
	auto matWVP0 = Transpose(matSpace* matView * matProj);
	mConstantBuffer.Update(&matWVP0);
	mSpace.BindPS(0);
	mMeshDomeBuffer.Draw();

	auto matWorld = Matrix4::RotationY(mRotation.y);
	auto matTranslation = Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f));
	auto matScl = Matrix4::Scaling(0.3f);
	auto matSun = matTranslation * matWorld;
	auto matWVP = Transpose(matScl*matSun* matView * matProj);
	mConstantBuffer.Update(&matWVP);
	mSun.BindPS(0);
	mMeshBuffer.Draw();

	auto matWorld1 = Matrix4::RotationY(mRotation.y*1.3f);
	auto matTranslation1 = Matrix4::Translation(Vector3(30.0f, 0.0f, 0.0f));
	auto matScl1 = Matrix4::Scaling(0.08f);
	auto matEarth = matTranslation1 * matWorld1;
	auto matWVP1 = Transpose(matScl1 * matEarth * matView * matProj);
	mConstantBuffer.Update(&matWVP1);
	mEarth.BindPS(0);
	mMeshBuffer.Draw();

	auto matWorld2 = Matrix4::RotationY(mRotation.y * 2.0f);
	auto moonSpin = Matrix4::RotationY(mRotation.y * 1.5f);
	auto matTranslation2 = Matrix4::Translation(Vector3(5.5f, 0.0f, 0.0f));
	auto matScl2 = Matrix4::Scaling(0.05f);
	auto matMoon = matWorld2 * matTranslation2 * moonSpin;
	auto matWVP2 = Transpose(matScl2 * matMoon * matEarth * matView * matProj);
	mConstantBuffer.Update(&matWVP2);
	mMoon.BindPS(0);
	mMeshBuffer.Draw();

	auto matWorld3 = Matrix4::RotationY(mRotation.y * 1.2f);
	auto matTranslation3 = Matrix4::Translation(Vector3(12.0f, 0.0f, 0.0f));
	auto matScl3 = Matrix4::Scaling(0.07f);
	auto matMercury = matTranslation3 * matWorld3;
	auto matWVP3 = Transpose(matScl3 * matMercury * matView * matProj);
	mConstantBuffer.Update(&matWVP3);
	mMercury.BindPS();
	mMeshBuffer.Draw();

	auto matWorld4= Matrix4::RotationY(mRotation.y * 0.54f);
	auto matTranslation4 = Matrix4::Translation(Vector3(17.0f, 0.0f, 0.0f));
	auto matScl4 = Matrix4::Scaling(0.06f);
	auto matVenus = matTranslation4 * matWorld4;
	auto matWVP4 = Transpose(matScl4 * matVenus * matView * matProj);
	mConstantBuffer.Update(&matWVP4);
	mVenus.BindPS(0);
	mMeshBuffer.Draw();

	auto matWorld5 = Matrix4::RotationY(mRotation.y * 0.79f);
	auto matTranslation5 = Matrix4::Translation(Vector3(45.0f, 0.0f, 0.0f));
	auto matScl5 = Matrix4::Scaling(0.04f);
	auto matMars = matTranslation5 * matWorld5;
	auto matWVP5 = Transpose(matScl5 * matMars * matView * matProj);
	mConstantBuffer.Update(&matWVP5);
	mMars.BindPS(0);
	mMeshBuffer.Draw();

	auto matWorld6 = Matrix4::RotationY(mRotation.y * 0.22f);
	auto matTranslation6 = Matrix4::Translation(Vector3(65.0f, 0.0f, 0.0f));
	auto matScl6 = Matrix4::Scaling(0.2f);
	auto matJupiter = matTranslation6 * matWorld6;
	auto matWVP6 = Transpose(matScl6 * matJupiter * matView * matProj);
	mConstantBuffer.Update(&matWVP6);
	mJupiter.BindPS(0);
	mMeshBuffer.Draw();

	auto matWorld7 = Matrix4::RotationY(mRotation.y * 0.31f);
	auto matTranslation7 = Matrix4::Translation(Vector3(80.0f, 0.0f, 0.0f));
	auto matScl7 = Matrix4::Scaling(0.175f);
	auto matSaturn = matTranslation7*matWorld7;
	auto matWVP7 = Transpose(matScl7 * matSaturn * matView * matProj);
	mConstantBuffer.Update(&matWVP7);
	mSaturn.BindPS(0);
	mMeshBuffer.Draw();

	auto matWorld8 = Matrix4::RotationY(mRotation.y * 0.4f);
	auto matTranslation8 = Matrix4::Translation(Vector3(95.0f, 0.0f, 0.0f));
	auto matScl8 = Matrix4::Scaling(0.09f);
	auto matUranos = matTranslation8 * matWorld8;
	auto matWVP8 = Transpose(matScl8 * matUranos * matView * matProj);
	mConstantBuffer.Update(&matWVP8);
	mUranos.BindPS(0);
	mMeshBuffer.Draw();

	auto matWorld9 = Matrix4::RotationY(mRotation.y * 0.54f);
	auto matTranslation9 = Matrix4::Translation(Vector3(105.0f, 0.0f, 0.0f));
	auto matScl9 = Matrix4::Scaling(0.06f);
	auto matNaptune = matTranslation9 * matWorld9;
	auto matWVP9 = Transpose(matScl9 * matNaptune * matView * matProj);
	mConstantBuffer.Update(&matWVP9);
	mNeptune.BindPS(0);
	mMeshBuffer.Draw();

	/*context->Draw(mVertices.size(), 0);*/ // This is for when we don't have an index buffer
}