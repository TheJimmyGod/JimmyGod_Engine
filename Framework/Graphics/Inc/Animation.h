#pragma once

#include "KeyFrame.h"

namespace JimmyGod::Graphics
{
	class Animation
	{
	public:
		Math::Vector3 GetPosition(float time) const;
		Math::Quaternion GetRotation(float time) const;
		Math::Vector3 GetScale(float time) const;
		//std::tuple<Math::Vector3, Math::Quaternion, Math::Vector3> GetTransform(float time) const;

		float GetDuration() const
		{
			return mTime;
		}

		void SetLooping(bool looping) { mLooping = looping; }
		bool IsLooping() const { return mLooping; }

		Math::Matrix4 GetTransform(float time) const;
		std::vector<float> mTimes;

	private:
		friend class AnimationBulider;
		friend class AnimationIO;

		PositionsKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;

		KeyFrames<float> mKeyFrames;

		bool mLooping = false;
		float mTime = 0.0f;
	};
	using Animations = std::vector<std::unique_ptr<Animation>>;

} // JimmyGod::Graphics