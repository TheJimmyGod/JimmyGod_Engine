#include "Precompiled.h"
#include "Cloth.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Physics;
using namespace JimmyGod::Math;

void Cloth::Initialize(const std::filesystem::path & path, uint32_t width, uint32_t height)
{
	PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f,-9.8f,0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.3f;
	settings.iterations = 1;
	mPhysicsWorld.Initialize(settings);

	mWidth = width; mHeight = height;
	mMesh = JimmyGod::Graphics::MeshBuilder::CreatePlanePX(mHeight, mWidth);
	mMeshBuffer.Initialize(mMesh, true);
	std::filesystem::path texturePath = L"../../Assets/Shaders/DoTexturing.fx";
	mVertexShader.Initialize(texturePath, JimmyGod::Graphics::VertexPX::Format);
	mPixelShader.Initialize(texturePath);

	mSampler.Initialize(JimmyGod::Graphics::Sampler::Filter::Anisotropic,
		JimmyGod::Graphics::Sampler::AddressMode::Border);
	mTexture.Initialize(path);
	mConstantBuffer.Initialize(sizeof(Matrix4));
}

void Cloth::Terminate()
{
	mConstantBuffer.Terminate();
	mSampler.Terminate();
	mTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void Cloth::Update(float deltaTime)
{
	if (!IsDisplay)return;
	mPhysicsWorld.Update(deltaTime);

	if (IsDisplay && (mMesh.vertices.size() == mParticles.size()))
	{
		for (size_t i = 0; i < mParticles.size(); ++i)
		{
			mMesh.vertices[i].position = mParticles[i]->position;
		}
	}
}

void Cloth::ShowCloth(const JimmyGod::Math::Vector3 & pos)
{
	IsDisplay = true;
	Vector3 offset = { 0.5f * mWidth + pos.x, 0.5f * mHeight + pos.y, 0.0f };
	mParticles.clear();
	mPhysicsWorld.Clear(true);

	for (uint32_t y = 0; y < mHeight; y++)
	{
		for (uint32_t x = 0; x < mWidth; x++)
		{
			auto p = new Particle({ Vector3{ -offset.x + (static_cast<float>(x)), offset.y - (static_cast<float>(y)),0.0f } });
			p->SetVelocity(Vector3{ RandomFloat(0.01f, 0.1f) });
			p->radius = 0.2f;
			p->bounce = 0.3f;
			mParticles.push_back(p);
			mPhysicsWorld.AddParticle(p);
		}
	}

	for (uint32_t y = 0; y < mHeight; y++)
	{
		for (uint32_t x = 0; x < mWidth; x++)
		{
			if (y == 0 || x == mWidth)
				mPhysicsWorld.AddConstraint(new Fixed(mParticles[GetIndex(x,y,mWidth)]));
			if (x + 1 < mWidth)
			{
				mPhysicsWorld.AddConstraint(new Spring(mParticles[GetIndex(x,y,mWidth)], mParticles[GetIndex(x+1,y,mWidth)]));

			}
			if (y + 1 < mHeight)
			{
				mPhysicsWorld.AddConstraint(new Spring(mParticles[GetIndex(x,y,mWidth)], mParticles[GetIndex(x,y+1, mWidth)]));
			}
		}
	}
}

void Cloth::Render(const JimmyGod::Graphics::Camera & camera, const JimmyGod::Math::Vector3& pos)
{
	if (!IsDisplay) return;

	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix();
	float cal = Magnitude(pos);
	auto matWorld = (cal == 0.0f) ? Matrix4::Identity : Matrix4::Scaling(0.8f) * Matrix4::Translation(pos);

	mConstantBuffer.BindVS();
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS();
	mTexture.BindPS();
	
	auto matrixViewProjection = JimmyGod::Math::Transpose(matWorld * view * projection);
	mConstantBuffer.Update(&matrixViewProjection);
	mMeshBuffer.Update(mMesh.vertices.data(), static_cast<uint32_t>(mMesh.vertices.size()));
	mMeshBuffer.Draw();
}

void Cloth::SetVelocity(const Vector3 & vel)
{
	for (auto& p : mParticles)
	{
		p->SetVelocity(vel);
	}
}
