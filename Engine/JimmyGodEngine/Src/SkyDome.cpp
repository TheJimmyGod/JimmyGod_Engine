#include "Precompiled.h"
#include "SkyDome.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void SkyDome::Intialize(const std::filesystem::path & path, float radius, uint32_t rings, uint32_t slices, const Vector3& pos)
{
	mPos = pos;

	mConstantBuffer.Initialize(sizeof(Matrix4));
	mTexture.Initialize(path);
	mMesh = MeshBuilder::CreateSpherePX(radius, rings, slices, true);
	mMeshBuffer.Initialize(mMesh);
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Clamp);
	mVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");
}

void SkyDome::Terminate()
{
	mConstantBuffer.Terminate();
	mVertexShader.Terminate();
	mTexture.Terminate();
	mMeshBuffer.Terminate();
	mPixelShader.Terminate();
	mSampler.Terminate();
}

void SkyDome::Update(const JimmyGod::Graphics::Camera & camera)
{
	mPos = camera.GetPosition();
}

void SkyDome::Render(const JimmyGod::Graphics::Camera& camera)
{
	auto matView = camera.GetViewMatrix();
	auto matProj = camera.GetPerspectiveMatrix();
	auto matSpace = Matrix4::Translation(mPos);
	auto matWVP = Transpose(matSpace* matView * matProj);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindPS(0);
	mTexture.BindVS(0);
	mTexture.BindPS(0);
	mConstantBuffer.Update(&matWVP);
	mConstantBuffer.BindVS(0);
	mConstantBuffer.BindPS(0);
	mMeshBuffer.Draw();
}
