#include "Precompiled.h"
#include "Spark.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Physics;
using namespace JimmyGod::Graphics;

void JimmyGod::Spark::Initialize(const std::filesystem::path & path, uint32_t amount, float radius, float spread)
{
	PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f,-9.8f,0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.3f;
	settings.iterations = 1;
	mPhysicsWorld.Initialize(settings);

	mRadius = radius;
	mMesh = JimmyGod::Graphics::MeshBuilder::CreateSpherePX(mRadius);
	mAmount = amount;
	mSpread = spread;
	mMeshBuffer.Initialize(mMesh, true);

	std::filesystem::path texturePath = L"../../Assets/Shaders/DoTexturing.fx";
	std::filesystem::path glowPath = L"../../Assets/Shaders/Effect.fx";
	// TODO: Create glowVertexShader and glowPixelShader
	mGlowVertexShader.Initialize(glowPath, "glowVSHorizontal1", JimmyGod::Graphics::VertexPX::Format);
	mGlowPixelShader.Initialize(glowPath, "GLOW_PS");

	mGlowVertexShader.Initialize(glowPath, "glowVSHorizontal2", JimmyGod::Graphics::VertexPX::Format);
	mGlowPixelShader.Initialize(glowPath, "GLOW_PS");

	mGlowVertexShader.Initialize(glowPath, "glowVSVertical1", JimmyGod::Graphics::VertexPX::Format);
	mGlowPixelShader.Initialize(glowPath, "GLOW_PS");

	mGlowVertexShader.Initialize(glowPath, "glowVSVertical2", JimmyGod::Graphics::VertexPX::Format);
	mGlowPixelShader.Initialize(glowPath, "GLOW_PS");

	mVertexShader.Initialize(texturePath,"VS" , JimmyGod::Graphics::VertexPX::Format);
	mPixelShader.Initialize(texturePath, "PS");
	mSampler.Initialize(JimmyGod::Graphics::Sampler::Filter::Linear,
		JimmyGod::Graphics::Sampler::AddressMode::Clamp);
	mTexture.Initialize(path);
	mConstantBuffer.Initialize(sizeof(Matrix4));
}

void JimmyGod::Spark::Terminate()
{
	mConstantBuffer.Terminate();
	mSampler.Terminate();
	mTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mGlowPixelShader.Terminate();
	mGlowVertexShader.Terminate();
	mPhysicsWorld.Clear();
}

void JimmyGod::Spark::Update(float deltaTime)
{
	if (!IsDisplay)return;
	mPhysicsWorld.Update(deltaTime);
	if (mTime > 0.0f) mTime -= deltaTime;
	else IsSummoned = false;
	if (IsSummoned == false) Clear();
}

void JimmyGod::Spark::ShowSpark(const JimmyGod::Math::Vector3 & pos, const JimmyGod::Math::Vector3& dir, float endTime)
{
	IsDisplay = true;
	mTime = endTime;
	IsSummoned = true;
	
	Vector3 NormalizedDir = Normalize(dir);

	mFoot = pos;
	Clear();
	for (uint32_t i = 0; i < mAmount; ++i)
	{
		auto p = new Particle({ Vector3{ mFoot.x,mFoot.y + 1.5f, mFoot.z } });
		p->SetVelocity(Vector3{ NormalizedDir.x + RandomFloat(-mSpread, mSpread),
			NormalizedDir.y + RandomFloat(-0.1f, 0.1f),
			NormalizedDir.z + RandomFloat(-mSpread, mSpread) });
		p->radius = mRadius;
		p->bounce = 0.3f;
		
		mParticles.push_back(p);
		mPhysicsWorld.AddParticle(p);
	}

}

void JimmyGod::Spark::Render(const JimmyGod::Graphics::Camera & camera)
{
	if (!IsDisplay) return;

	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix();
	// for statement
	mConstantBuffer.BindVS();

	mSampler.BindPS(0);
	mTexture.BindPS(0);



	if (mParticles.empty())
		return;
	for (uint32_t i = 0; i < mAmount; ++i)
	{
		auto matWorld = Matrix4::Translation(mParticles[i]->position);
		auto matrixViewProjection = JimmyGod::Math::Transpose(matWorld * view * projection);
		mConstantBuffer.Update(&matrixViewProjection);
		mGlowPixelShader.Bind();
		mGlowVertexShader.Bind();

		mVertexShader.Bind();
		mPixelShader.Bind();
		mMeshBuffer.Update(mMesh.vertices.data(), static_cast<uint32_t>(mMesh.vertices.size()));
		if (IsDebugUI == false)
			mMeshBuffer.Draw();
	}

}

void JimmyGod::Spark::DebugUI(bool debug)
{
	IsDebugUI = debug;

	if (IsDebugUI == true)
	{
		mPhysicsWorld.DebugDraw();
	}
}