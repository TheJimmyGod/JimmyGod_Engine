#include "Precompiled.h"
#include "Bomb.h"
#include "Spark.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Physics;

void JimmyGod::Bomb::Initialize(const std::filesystem::path & path, float radius)
{
	PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f,-9.8f,0.0f };
	settings.timeStep = 1.0f / 60.0f;
	settings.drag = 0.3f;
	settings.iterations = 1;
	mPhysicsWorld.Initialize(settings);
	mPhysicsWorld.AddPlane({ Vector3::YAxis, 0.0f });

	mRadius = radius;
	mMesh = JimmyGod::Graphics::MeshBuilder::CreateSpherePX(mRadius);
	mSpark.Initialize("../../Assets/Textures/Color.png", 25, 0.05f);

	mMeshBuffer.Initialize(mMesh, true);
	std::filesystem::path texturePath = L"../../Assets/Shaders/DoTexturing.fx";
	mVertexShader.Initialize(texturePath, JimmyGod::Graphics::VertexPX::Format);
	mPixelShader.Initialize(texturePath);

	mSampler.Initialize(JimmyGod::Graphics::Sampler::Filter::Anisotropic,
		JimmyGod::Graphics::Sampler::AddressMode::Border);
	mTexture.Initialize(path);
	mConstantBuffer.Initialize(sizeof(Matrix4));
}

void JimmyGod::Bomb::Terminate()
{
	mConstantBuffer.Terminate();
	mSampler.Terminate();
	mTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mPhysicsWorld.Clear();
	mSpark.Terminate();
	mParticle = nullptr;
	delete mParticle;
}

void JimmyGod::Bomb::Update(float deltaTime)
{
	if (!IsDisplay)return;
	mPhysicsWorld.Update(deltaTime);
	mSpark.Update(deltaTime);
	if (mTime > 0.0f)
	{
		if (mSec < 0.0f)
		{
			mSpark.ShowSpark(Vector3{ mParticle->position.x,mParticle->position.y - 1.0f,mParticle->position.z}, mParticle->position.YAxis / deltaTime, 0.05f);
			mSec = 0.12f;
		}
		mTime -= deltaTime;
		mSec -= deltaTime;
	}
	else
		IsSummoned = false;
	if (IsSummoned == false && mParticle != nullptr)
	{
		mSpark.Clear();
		mParticle = nullptr;
		mPhysicsWorld.Clear(true);
		mSpark.Active(false);
		Active(false);
	}

}

void JimmyGod::Bomb::ShowBomb(const JimmyGod::Math::Vector3 & pos, const JimmyGod::Math::Vector3 & dir, float endTime, 
	float radius, float bounce, float power, float throwDist)
{
	IsDisplay = true;
	IsSummoned = false;

	mTime = endTime;

	Vector3 NormalizedDir = Normalize(dir);
	mPosition = pos;
	mParticle = nullptr;
	mPhysicsWorld.Clear(true);
	mRadius = radius;
	auto p = new Particle({ Vector3{ mPosition.x + (NormalizedDir.x * throwDist),mPosition.y + 5.5f, mPosition.z + (NormalizedDir.z * throwDist) } });
	p->SetVelocity(Vector3{ NormalizedDir.x * power,
		NormalizedDir.y,
		NormalizedDir.z * power});
	p->radius = mRadius;
	p->bounce = bounce;
	mParticle = std::move(p);
	
	mPhysicsWorld.AddParticle(p);
	IsSummoned = true;
}

void JimmyGod::Bomb::Render(const JimmyGod::Graphics::Camera & camera)
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
	
	if (mParticle == nullptr)
		return;
	auto matWorld = Matrix4::Translation(mParticle->position);
	auto matrixViewProjection = JimmyGod::Math::Transpose(matWorld * view * projection);
	mConstantBuffer.Update(&matrixViewProjection);
	mMeshBuffer.Update(mMesh.vertices.data(), static_cast<uint32_t>(mMesh.vertices.size()));
	if (IsDebugUI == false)
		mMeshBuffer.Draw();
	mSpark.Render(camera);
}

void JimmyGod::Bomb::DebugUI(bool debug)
{
	IsDebugUI = debug;

	if (IsDebugUI == true)
	{
		mPhysicsWorld.DebugDraw();
	}
	mSpark.DebugUI(IsDebugUI);
}
