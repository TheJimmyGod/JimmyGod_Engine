#include "Precompiled.h"
#include "Constraints.h"

using namespace JimmyGod;
using namespace JimmyGod::Physics;

Spring::Spring(Particle * a, Particle * b, float restLength)
	: mParticleA(a), mParticleB(b), mRestLength(restLength)
{
	if (restLength <= 0.0f)
		mRestLength = Math::Magnitude(a->position - b->position);
}

void Spring::Apply() const
{
	Math::Vector3 delta = mParticleB->position - mParticleA->position;
	float dist = Math::Magnitude(delta);
	float diff = (dist - mRestLength) / (dist * (mParticleA->invMass + mParticleB->invMass));
	mParticleA->position += delta * diff * mParticleA->invMass;
	mParticleB->position -= delta * diff * mParticleB->invMass;
}

void Spring::DebugDraw() const
{
	Graphics::SimpleDraw::AddLine(
		mParticleA->position,
		mParticleB->position,
		Graphics::Colors::Cyan
	);
}

Fixed::Fixed(Particle * p) : Fixed(p,p->position)
{
}

Fixed::Fixed(Particle * p, const Math::Vector3 & position)
	: mParticle(p), mPosition(position)
{
}

void Fixed::Apply() const
{
	mParticle->SetPosition(mPosition);
}

void Fixed::DebugDraw() const
{
	Graphics::SimpleDraw::AddAABB(mPosition, mParticle->radius, Graphics::Colors::Cyan);
}

void Fixed::SetPosition(const Math::Vector3 & position)
{
	mPosition = position;
}

// Homework:
// Add the following:
// - Tetrahedrons
// - Cubes (how many constraints do you need?)
// - Ball and Chain
// - Cloth
// - And if you are up for the challenge, build 

//void Cloth::Initialize(const std::filesystem::path& path,
//	uint32_t width, uint32_t height)
//{
//	PhysicsWorld::Settings settings;
//	settings.gravity = { 0.0f,-9.8f,0.0f };
//	settings.timeStep = 1.0f / 60.0f;
//	settings.drag = 0.3f;
//	settings.iterations = 1;
//	mPhysicsWorld.Initialize(settings);
//
//	mWidth = width; mHeight = height;
//	mMesh = JimmyGod::Graphics::MeshBuilder::CreatePlanePX(width, height);
//	mMeshBuffer.Initialize(mMesh, true);
//	std::filesystem::path texturePath = L"../../Assets/Shaders/DoTexturing.fx";
//	mVertexShader.Initialize(texturePath, JimmyGod::Graphics::VertexPX::Format);
//	mPixelShader.Initialize(texturePath);
//
//	mSampler.Initialize(JimmyGod::Graphics::Sampler::Filter::Anisotropic,
//		JimmyGod::Graphics::Sampler::AddressMode::Border);
//	mTexture.Initialize(path);
//	mConstantBuffer.Initialize(sizeof(Matrix4));
//}
//
//void Cloth::Terminate()
//{
//	mConstantBuffer.Terminate();
//	mSampler.Terminate();
//	mTexture.Terminate();
//	mPixelShader.Terminate();
//	mVertexShader.Terminate();
//	mMeshBuffer.Terminate();
//}
//
//void Cloth::Update(float deltaTime)
//{
//	mPhysicsWorld.Update(deltaTime);
//
//	if (IsDisplay && (mMesh.vertices.size() == mParticles.size()))
//	{
//		for (size_t i = 0; i < mParticles.size(); i++)
//		{
//			mMesh.vertices[i].position = mParticles[i]->position;
//		}
//	}
//
//}
//
//void Cloth::Render(const JimmyGod::Graphics::Camera& camera)
//{
//	if (!IsDisplay) return;
//
//	auto view = camera.GetViewMatrix();
//	auto projection = camera.GetPerspectiveMatrix();
//
//	mConstantBuffer.BindVS();
//	mVertexShader.Bind();
//	mPixelShader.Bind();
//	mSampler.BindPS();
//	mTexture.BindPS();
//
//	auto matrixViewProjection = JimmyGod::Math::Transpose(view * projection);
//	mConstantBuffer.Update(&matrixViewProjection);
//	mMeshBuffer.Update(mMesh.vertices.data(),
//		static_cast<uint32_t>(mMesh.vertices.size()));
//	mMeshBuffer.Draw();
//}
