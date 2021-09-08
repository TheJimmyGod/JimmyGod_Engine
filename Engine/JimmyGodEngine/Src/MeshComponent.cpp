#include "Precompiled.h"
#include "MeshComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

META_DERIVED_BEGIN(MeshComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(mPath, "Path")
		META_FIELD(mDiffusePath, "Diffuse")
		META_FIELD(mSpecularPath, "Specular")
		META_FIELD(mDisplacementPath, "Displacement")
		META_FIELD(mNormalPath, "Normal")
		META_FIELD(mAOPath, "AO")
	META_FIELD_END
META_CLASS_END

void MeshComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	OBJLoader::Load(mPath, 0.4f, mMesh);
	mMeshBuffer.Initialize(mMesh);

	mVertexShader.Initialize("../../Assets/Shaders/DoPhongShading.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoPhongShading.fx");

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	if(mDiffusePath != "None")
		mDiffuseMap.Initialize(mDiffusePath);
	if(mSpecularPath != "None")
		mSpecularMap.Initialize(mSpecularPath);
	if (mDisplacementPath != "None")
		mDiffuseMap.Initialize(mDisplacementPath);
	if(mNormalPath != "None")
		mNormalMap.Initialize(mNormalPath);
	if(mAOPath != "None")
		mAOMap.Initialize(mAOPath);
}
void MeshComponent::Terminate()
{
	mMeshBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mSampler.Terminate();

	if(mDiffuseMap.IsInitialized())
		mDiffuseMap.Terminate();
	if(mSpecularMap.IsInitialized())
		mSpecularMap.Terminate();
	if(mNormalMap.IsInitialized())
		mNormalMap.Terminate();
	if(mAOMap.IsInitialized())
		mAOMap.Terminate();
}
void MeshComponent::Update(float deltaTime)
{

}
void MeshComponent::Render()
{
	if(!isDebugUI)
		mMeshBuffer.Draw();
}
void MeshComponent::DebugUI()
{
	if (isDebugUI)
		JimmyGod::Graphics::SimpleDraw::AddAABB(AABB(mTransformComponent->GetPosition(), mColliderComponent->radius), Colors::Aqua);
}

void JimmyGod::MeshComponent::Bind()
{
	if (isDebugUI)
		return;
	if (mDiffuseMap.IsInitialized()) mDiffuseMap.BindPS(0);
	if (mSpecularMap.IsInitialized()) mSpecularMap.BindPS(1);
	if (mDisplacementMap.IsInitialized())mDisplacementMap.BindVS(2);
	if (mNormalMap.IsInitialized()) mNormalMap.BindPS(3);
	if (mAOMap.IsInitialized())mAOMap.BindPS(4);

	mVertexShader.Bind();
	mPixelShader.Bind();
}
