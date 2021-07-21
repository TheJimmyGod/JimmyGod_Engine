#include "Precompiled.h"
#include "Cloth.h"

#include "TransformComponent.h"
#include "ModelComponent.h"
using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Physics;
using namespace JimmyGod::Math;

void Cloth::Initialize(const std::filesystem::path& path, uint32_t width, uint32_t height)
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
	mRasterizerState.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	mSampler.Initialize(JimmyGod::Graphics::Sampler::Filter::Anisotropic,
		JimmyGod::Graphics::Sampler::AddressMode::Border);
	mTexture.Initialize(path);
	mConstantBuffer.Initialize(sizeof(Matrix4));
}

void Cloth::Initialize(GameObject& gameObject, const std::filesystem::path & path, uint32_t width, uint32_t height)
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
	mRasterizerState.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	mSampler.Initialize(JimmyGod::Graphics::Sampler::Filter::Anisotropic,
		JimmyGod::Graphics::Sampler::AddressMode::Border);
	mTexture.Initialize(path);
	mConstantBuffer.Initialize(sizeof(Matrix4));

	mOwner = &gameObject;
}

void Cloth::Terminate()
{
	mConstantBuffer.Terminate();
	mSampler.Terminate();
	mTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mPhysicsWorld.Clear();
	mRasterizerState.Terminate();

	if(mOwner != nullptr)
		mOwner->Terminate();
}

void Cloth::Update(float deltaTime, bool isCloak)
{
	if (!IsDisplay)return;
	mPhysicsWorld.Update(deltaTime);

	if (isCloak == true)
	{
		if (mFixed.empty())
			return;
		for (size_t i = 0; i < mFixed.size(); ++i)
		{
			mRot = Matrix4::RotationAxis(mCenter.YAxis, 180.0f * Constants::DegToRad);
			if (i == 0)
			{
				mFixedParticles[i]->SetPosition(mRight);
				mParticles[mFixed[i]]->SetPosition(mRight);
			}
			else if (i == mFixed.size() - 1)
			{
				mFixedParticles[i]->SetPosition(mLeft);
				mParticles[mFixed[i]]->SetPosition(mLeft);
			}
			else
			{
				mFixedParticles[i]->SetPosition(mCenter);
				mParticles[mFixed[i]]->SetPosition(mCenter);
			}
		}
		SetPosition();
	}

	if (IsDisplay && (mMesh.vertices.size() == mParticles.size()))
	{
		for (size_t i = 0; i < mParticles.size(); ++i)
		{
			mMesh.vertices[i].position = mParticles[i]->position;
		}
	}

}

void Cloth::ShowCloth()
{
	const std::vector<Matrix4> boneMatrix = mOwner->GetComponent<ModelComponent>()->GetAnimator().GetBoneMatrices();
	const Bone* mNeck = FindBone(mOwner->GetComponent<ModelComponent>()->GetModel().mSkeleton, "mixamorig1:Neck");
	const Vector3 pos = mOwner->GetComponent<TransformComponent>()->GetPosition() + (GetTranslation(boneMatrix[mNeck->index]) * mOwner->GetComponent<TransformComponent>()->GetScale());

	IsDisplay = true;
	Vector3 offset = { 0.5f * mWidth + pos.x, 0.5f * mHeight + pos.y, pos.z };
	mParticles.clear();

	mPhysicsWorld.Clear(true);
	mFixedParticles.clear();
	mFixed.clear();
	mOffset = offset;
	for (uint32_t y = 0; y < mHeight; y++)
	{
		for (uint32_t x = 0; x < mWidth; x++)
		{
			auto p = new Particle({ Vector3{ -offset.x + (static_cast<float>(x)), offset.y - (static_cast<float>(y)), offset.z } });
			p->SetVelocity(Vector3{ RandomFloat(1.0f, 1.5f) });
			p->radius = 0.1f;
			p->bounce = 0.4f;
			mParticles.push_back(p);
			mPhysicsWorld.AddParticle(p);

		}
	}

	for (uint32_t y = 0; y < mHeight; y++)
	{
		for (uint32_t x = 0; x < mWidth; x++)
		{
			if (y == 0 || x == mWidth)
			{
				auto f = new Fixed(mParticles[GetIndex(x, y, mWidth)]);
				mPhysicsWorld.AddConstraint(f);
				mFixed.push_back(x + y);
				mFixedParticles.push_back(f);
			}
			if (x + 1 < mWidth)
			{
				auto c = new Spring(mParticles[GetIndex(x, y, mWidth)], mParticles[GetIndex(x + 1, y, mWidth)]);
				mPhysicsWorld.AddConstraint(c);
			}
			if (y + 1 < mHeight)
			{
				auto c = new Spring(mParticles[GetIndex(x, y, mWidth)], mParticles[GetIndex(x, y + 1, mWidth)]);
				mPhysicsWorld.AddConstraint(c);
			}
		}
	}
}

void JimmyGod::Cloth::ShowClothWithoutGameObject(const JimmyGod::Math::Vector3& pos)
{
	IsDisplay = true;
	Vector3 offset = { 0.5f * mWidth + pos.x, 0.5f * mHeight + pos.y, pos.z };
	mParticles.clear();

	mPhysicsWorld.Clear(true);
	mFixedParticles.clear();
	mFixed.clear();
	mOffset = offset;
	for (uint32_t y = 0; y < mHeight; y++)
	{
		for (uint32_t x = 0; x < mWidth; x++)
		{
			auto p = new Particle({ Vector3{ -offset.x + (static_cast<float>(x)), offset.y - (static_cast<float>(y)), offset.z } });
			p->SetVelocity(Vector3{ RandomFloat(1.0f, 1.5f) });
			p->radius = 0.1f;
			p->bounce = 0.4f;
			mParticles.push_back(p);
			mPhysicsWorld.AddParticle(p);

		}
	}

	for (uint32_t y = 0; y < mHeight; y++)
	{
		for (uint32_t x = 0; x < mWidth; x++)
		{
			if (y == 0 || x == mWidth)
			{
				auto f = new Fixed(mParticles[GetIndex(x, y, mWidth)]);
				mPhysicsWorld.AddConstraint(f);
				mFixed.push_back(x + y);
				mFixedParticles.push_back(f);
			}
			if (x + 1 < mWidth)
			{
				auto c = new Spring(mParticles[GetIndex(x, y, mWidth)], mParticles[GetIndex(x + 1, y, mWidth)]);
				mPhysicsWorld.AddConstraint(c);
			}
			if (y + 1 < mHeight)
			{
				auto c = new Spring(mParticles[GetIndex(x, y, mWidth)], mParticles[GetIndex(x, y + 1, mWidth)]);
				mPhysicsWorld.AddConstraint(c);
			}
		}
	}
}

void Cloth::Render(const JimmyGod::Graphics::Camera & camera)
{
	if (!IsDisplay) return;

	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix();

	mConstantBuffer.BindVS();
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS();
	mTexture.BindPS();
	mRasterizerState.Set();

	auto matrixViewProjection = JimmyGod::Math::Transpose(view * projection);
	mConstantBuffer.Update(&matrixViewProjection);
	mMeshBuffer.Update(mMesh.vertices.data(), static_cast<uint32_t>(mMesh.vertices.size()));
	if(IsDebugUI == false)
		mMeshBuffer.Draw();
	mRasterizerState.Clear();
}

void Cloth::DebugUI(bool debug)
{
	IsDebugUI = debug;

	if (IsDebugUI == true)
		mPhysicsWorld.DebugDraw();
}

void Cloth::SetVelocity(const Vector3 & vel)
{
	mVelocity = vel;
	for (auto& p : mParticles)
		p->SetVelocity(mVelocity);
}

void Cloth::SetPosition()
{
	const Vector3 position = mOwner->GetComponent<TransformComponent>()->GetPosition();
	const Matrix4 rotation = Matrix4::RotationQuaternion(mOwner->GetComponent<TransformComponent>()->GetRotation());
	const std::vector<Matrix4> boneMatrix = mOwner->GetComponent<ModelComponent>()->GetAnimator().GetBoneMatrices();

	const Bone* mLeftShoulder = FindBone(mOwner->GetComponent<ModelComponent>()->GetModel().mSkeleton, "mixamorig1:LeftArm_$AssimpFbx$_Translation");
	const Bone* mRightShoulder = FindBone(mOwner->GetComponent<ModelComponent>()->GetModel().mSkeleton, "mixamorig1:RightArm_$AssimpFbx$_Translation");
	const Bone* mNeck = FindBone(mOwner->GetComponent<ModelComponent>()->GetModel().mSkeleton, "mixamorig1:Neck");

	mCenter = position + Vector3{ 0.0f, 0.2f, 0.0f } + GetTranslation(boneMatrix[mNeck->index] * rotation) * 0.04f;
	mLeft = position + Vector3{ 0.0f, 0.2f, 0.0f } + GetTranslation(boneMatrix[mLeftShoulder->index] * rotation) * 0.04f;
	mRight = position + Vector3{ 0.0f, 0.2f, 0.0f } + GetTranslation(boneMatrix[mRightShoulder->index] * rotation) * 0.04f;
}
