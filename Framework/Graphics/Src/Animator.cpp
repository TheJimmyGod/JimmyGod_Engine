#include "Precompiled.h"
#include "Animator.h"

using namespace JimmyGod;
using namespace JimmyGod::Graphics;

void Animator::Initialize(const Model & model)
{
	mModel = &model;
	mBoneMatrices.resize(mModel->mSkeleton.bones.size());
}

void Animator::Terminate()
{
	mModel = nullptr;
}

void Animator::ComputeBindPose()
{
	UpdateBoneRecursive(mModel->mSkeleton.root, mBoneMatrices);
}

void Animator::PlayAnimation(int index)
{
	mClipIndex = index;
	mBlendDuration = 0.0f;
}

void Animator::BlendTo(int index, float duration)
{
	mBlendWeight = 0.0f;
	mBlendTime = 0.0f;
	mBlendDuration = duration;
	mBlendIndex = index;
}

void Animator::Update(float deltaTime)
{
	auto& animationClip = mModel->mAnimationSet.clips[mClipIndex];
	mClipDuration = animationClip->duration;
	mTimer += deltaTime * animationClip->ticksPerSecond * mSpeed;
	if (mTimer > mClipDuration)
	{
		if (mStop)
			return;
		mTimer -= mClipDuration;
	}
	UpdateBoneRecursive(mModel->mSkeleton.root, mBoneMatrices, *animationClip, mTimer);

	if (mBlendDuration > 0.0f)
	{
		auto& blendClip = mModel->mAnimationSet.clips[mBlendIndex];
		mBlendTimer += deltaTime * blendClip->ticksPerSecond;
		if (mBlendTimer > blendClip->duration)
			mBlendTimer -= blendClip->duration;

		mBlendTime += deltaTime;
		if (mBlendTime > mBlendDuration)
		{
			mClipIndex = mBlendIndex;
			mTimer = mBlendTimer;
			mBlendWeight = 1.0f;
			mBlendDuration = 0.0f;
		}
		else
		{
			mBlendWeight = mBlendTime / mBlendDuration;
		}
		std::vector<JimmyGod::Math::Matrix4> targetMatrices(mBoneMatrices.size());

		UpdateBoneRecursive(mModel->mSkeleton.root, targetMatrices, *blendClip, mBlendTimer);

		for (size_t i = 0; i < mBoneMatrices.size(); ++i)
		{
			mBoneMatrices[i] = mBoneMatrices[i] * (1.0f - mBlendWeight) +
				targetMatrices[i] * mBlendWeight;
		}
	}
}
