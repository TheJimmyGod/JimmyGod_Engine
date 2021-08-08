#include "Precompiled.h"
#include "ModelComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

META_DERIVED_BEGIN(ModelComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(mPath, "Path")
	META_FIELD_END
META_CLASS_END

void ModelComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mModel.Initialize(mPath);
	mAnimator.Initialize(mModel);
	mBoneTransformBuffer.Initialize();
	mAnimator.SetSpeed(0.5f);
}

void ModelComponent::Terminate()
{
	mModel.Terminate();
	mAnimator.Terminate();
}

void ModelComponent::Update(float deltaTime)
{
	if(isActive)
		mAnimator.Update(deltaTime);
}

void ModelComponent::Render()
{
	if (isActive)
	{
		mBoneTransformBuffer.BindVS(5);
		BoneTransformData boneTransformData{};
		for (size_t i = 0; i < mAnimator.GetBoneMatrices().size(); ++i)
			boneTransformData.BoneTransforms[i] = Transpose(mModel.mSkeleton.bones[i]->offsetTransform * mAnimator.GetBoneMatrices()[i]);
		if (!isDebugUI)
			mModel.Render();
		else
			DrawSkeleton(mModel.mSkeleton.root, mAnimator.GetBoneMatrices(),
				mTransformComponent->GetPosition(),
				mTransformComponent->GetScale(),
				Matrix4::RotationQuaternion(mTransformComponent->GetRotation()));

		mBoneTransformBuffer.Update(&boneTransformData);
	}
}

void ModelComponent::DebugUI()
{

}