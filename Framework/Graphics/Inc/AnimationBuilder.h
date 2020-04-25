#pragma once

#include"Animation.h"

namespace JimmyGod::Graphics
{
	class AnimationBulider
	{
	public:
		AnimationBulider& AddTime(float time);
		AnimationBulider& AddPositionKey(Math::Vector3 position, float time);
		AnimationBulider& AddRotationKey(Math::Quaternion rotation, float time);
		AnimationBulider& AddScale(Math::Vector3 scale, float time);
		Math::Matrix4 GetAnimation(float time);
		Animation Build();

	private:
		Animation mAnimation;
		float mTime;
	};
}