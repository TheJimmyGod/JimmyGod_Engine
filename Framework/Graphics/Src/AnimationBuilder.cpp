#include "Precompiled.h"
#include"AnimationBuilder.h"
#include"Animation.h"

using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

AnimationBulider & AnimationBulider::AddTime(float time)
{
	mTime = time;
	mAnimation.mTimes.push_back(time);
	return *this;
}

AnimationBulider & AnimationBulider::AddPositionKey(Vector3 position, float time)
{
	mAnimation.mPositionKeys.push_back({ position, time });
	return *this;
}

AnimationBulider & AnimationBulider::AddRotationKey(Quaternion rotation, float time)
{
	mAnimation.mRotationKeys.push_back({ rotation, time });
	return *this;
}

AnimationBulider & AnimationBulider::AddScale(Vector3 scale, float time)
{
	mAnimation.mScaleKeys.push_back({ scale,time });
	return *this;
}

Matrix4 AnimationBulider::GetAnimation(float time)
{
	return mAnimation.GetTransform(time);
}

Animation AnimationBulider::Build()
{
	return mAnimation;
}
