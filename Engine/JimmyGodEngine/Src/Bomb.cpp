#include "Precompiled.h"
#include "Bomb.h"

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

	mParticle = nullptr;
	delete mParticle;
}

void JimmyGod::Bomb::Update(float deltaTime)
{
	if (!IsDisplay)return;
	mPhysicsWorld.Update(deltaTime);
	if (mTime > 0.0f)
		mTime -= deltaTime;
	else
		IsSummoned = false;
	if (IsSummoned == false)
	{
		mParticle = nullptr;
		mPhysicsWorld.Clear(true);
	}

}

void JimmyGod::Bomb::ShowBomb(const JimmyGod::Math::Vector3 & pos, const JimmyGod::Math::Vector3 & dir, float endTime)
{
	IsDisplay = true;
	IsSummoned = false;

	mTime = endTime;

	Vector3 NormalizedDir = Normalize(dir);
	float offset = 0.2f;
	float throwOffset = 7.0f;
	mPosition = pos;
	mParticle = nullptr;
	mPhysicsWorld.Clear(true);

	auto p = new Particle({ Vector3{ mPosition.x + (NormalizedDir.x * throwOffset),mPosition.y + 5.5f, mPosition.z + (NormalizedDir.z * throwOffset) } });
	p->SetVelocity(Vector3{ NormalizedDir.x * offset,
		NormalizedDir.y,
		NormalizedDir.z * offset });
	p->radius = mRadius;
	p->bounce = 0.7f;
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
}

void JimmyGod::Bomb::DebugUI(bool debug)
{
	IsDebugUI = debug;

	if (IsDebugUI == true)
	{
		mPhysicsWorld.DebugDraw();
	}
}
