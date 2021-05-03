#include "Precompiled.h"
#include "Planet.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void Planet::Initialize(const std::filesystem::path & path, const Math::Vector3 & pos, float radius, float rings, float slices)
{
	mPos = pos;
	mTexture.Initialize(path);
	mMesh = MeshBuilder::CreateSphere(radius, rings, slices, false);
	mMeshBuffer.Initialize(mMesh);
	mTransformBuffer.Initialize();
	mSettings.bumpMapWeight = { 0.2f };
	mSettingsBuffer.Initialize();

	mConstantBuffer.Initialize(sizeof(Matrix4));
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Clamp);

	mVertexShader.Initialize("../../Assets/Shaders/Earth.fx", "VSEarth", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Earth.fx", "PSEarth");
}

void Planet::Terminate()
{
	mConstantBuffer.Terminate();
	mTexture.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mSampler.Terminate();
	mMeshBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void Planet::Update(float deltaTime)
{
	mRot += deltaTime;
}

void Planet::Render(const Graphics::Camera & camera, float range, float scl, const JimmyGod::Math::Matrix4& mat)
{
	auto matView = camera.GetViewMatrix();
	auto matProj = camera.GetPerspectiveMatrix();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mConstantBuffer.BindPS();
	mSampler.BindPS();

	auto matWorld = Matrix4::RotationY(mRot.y * range);
	auto matSpin = Matrix4::RotationY(mRot.y * 0.105f);
	auto matTranslation = Matrix4::Translation(mPos);
	auto matScl = Matrix4::Scaling(scl);
	auto matPlanet = matWorld * matTranslation * matSpin;
	auto matWVP = Transpose(matScl * matPlanet * mat * matView * matProj);
	mConstantBuffer.Update(&matWVP);
	mConstantBuffer.BindPS(0);
	mConstantBuffer.BindVS(0);

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = matWVP;
	transformData.viewPosition = camera.GetPosition();

	mTransformBuffer.Update(&transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mSettingsBuffer.Update(&mSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mTexture.BindPS(0);
	mTexture.BindVS(0);
	mMeshBuffer.Draw();
}
