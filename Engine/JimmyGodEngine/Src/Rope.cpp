#include "Precompiled.h"
#include "Rope.h"

using namespace JimmyGod;
using namespace JimmyGod::Physics;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void JimmyGod::Rope::Initialize(const std::filesystem::path & path, float radius, uint32_t length)
{
	PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f,-9.8f,0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.3f;
	settings.iterations = 1;
	mPhysicsWorld.Initialize(settings);
	mPhysicsWorld.AddPlane({ Vector3::YAxis, 0.0f });

	mRadius = radius;
	mLength = length;
	mMesh = JimmyGod::Graphics::MeshBuilder::CreateSpherePX(mRadius);

	mMeshBuffer.Initialize(mMesh, true);
	std::filesystem::path texturePath = L"../../Assets/Shaders/DoTexturing.fx";
	mVertexShader.Initialize(texturePath, JimmyGod::Graphics::VertexPX::Format);
	mPixelShader.Initialize(texturePath);

	mSampler.Initialize(JimmyGod::Graphics::Sampler::Filter::Anisotropic,
		JimmyGod::Graphics::Sampler::AddressMode::Border);
	mTexture.Initialize(path);
	mConstantBuffer.Initialize(sizeof(Matrix4));
}

void JimmyGod::Rope::Terminate()
{
	mConstantBuffer.Terminate();
	mSampler.Terminate();
	mTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mPhysicsWorld.Clear();
	mFixed = nullptr;
	delete mFixed;
}

void JimmyGod::Rope::Update(float deltaTime, const Vector3& pos)
{
	if (!IsDisplay)return;
	mPhysicsWorld.Update(deltaTime);
	if (mTime > 0.0f)
	{
		if (mTime > 0.75f)
		{
			for (auto& p : mParticles)
				p->SetVelocity(mVelocity * 1.5f);
			mFixed->SetPosition(pos);
			mParticles[0]->SetPosition(pos);
		}
		mTime -= deltaTime;
	}
	else
	{
		if (IsRopeEnded)
		{
			Active(false);
			IsRopeEnded = false;
		}
	}
}

void JimmyGod::Rope::ShowRope(const JimmyGod::Math::Vector3 & pos, const JimmyGod::Math::Vector3 & dir,
	float radius, float power, uint32_t length, float bounce, float dist, float LifeTime)
{
	IsDisplay = true;
	IsRopeEnded = false;
	mPosition = pos;
	Vector3 NormalizedDir = Normalize(dir);
	mVelocity = Vector3{ NormalizedDir.x * power,
		NormalizedDir.y,
		NormalizedDir.z * power };
	mParticles.clear();
	mPhysicsWorld.Clear(true);
	mFixed = nullptr;
	mLength = length;
	mRadius = radius;
	for (uint32_t i = 0; i < mLength; ++i)
	{
		if (i != 0)
		{
			auto p1 = new Particle({ Vector3{ mPosition.x + (NormalizedDir.x * dist),mPosition.y + 5.5f + (static_cast<float>(i) * 0.2f),
				mPosition.z + (NormalizedDir.z * dist) } });
			p1->SetVelocity(Vector3{ NormalizedDir});
			p1->radius = mRadius;
			p1->bounce = bounce;
			mPhysicsWorld.AddParticle(p1);
			mParticles.push_back(p1);
		}
		else
		{
			auto p2 = new Particle({ Vector3{ mPosition.x + (NormalizedDir.x * dist),mPosition.y + 5.5f, mPosition.z + (NormalizedDir.z * dist) } });
			p2->SetVelocity(Vector3{ NormalizedDir.x,
		NormalizedDir.y,
		NormalizedDir.z });
			p2->radius = mRadius;
			p2->bounce = bounce;
			mPhysicsWorld.AddParticle(p2);
			mParticles.push_back(p2);
		}
	}
	mTime = LifeTime;
	IsRopeEnded = true;
	for (size_t i = 0; i < mParticles.size(); ++i)
	{
		if (i != 0)
		{
			mPhysicsWorld.AddConstraint(new Spring(mParticles[i], mParticles[i - 1]));
		}
		else
		{
			auto f = new Fixed(mParticles[i]);
			mPhysicsWorld.AddConstraint(f);
			mFixed = f;
		}
	}

}

void JimmyGod::Rope::Render(const JimmyGod::Graphics::Camera & camera)
{
	if (!IsDisplay) return;

	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix();
	// for statement
	mConstantBuffer.BindVS();
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS();
	mTexture.BindPS();
	if (mParticles.empty())
		return;
	for (uint32_t i = 0; i < mLength; ++i)
	{
		auto matWorld = Matrix4::Translation(mParticles[i]->position);
		auto matrixViewProjection = JimmyGod::Math::Transpose(matWorld * view * projection);
		mConstantBuffer.Update(&matrixViewProjection);
		mMeshBuffer.Update(mMesh.vertices.data(), static_cast<uint32_t>(mMesh.vertices.size()));
		if (IsDebugUI == false)
			mMeshBuffer.Draw();
	}

}

void JimmyGod::Rope::DebugUI(bool debug)
{
	if (!IsDisplay)
		return;
	IsDebugUI = debug;

	if (IsDebugUI == true)
	{
		mPhysicsWorld.DebugDraw();
	}
}
