#pragma once
#include "Model.h"

namespace JimmyGod::Graphics
{
	class Animator
	{
	public:
		void Initialize(const Model& model);
		void Terminate();
		void ComputeBindPose();
		void PlayAnimation(int index);
		void BlendTo(int index, float duration);

		void Update(float deltaTime);

		float GetTime() const
		{
			return mTimer;
		}

		float GetCurrentAniTime() const
		{
			return mCurrentTimer;
		}

		float GetCurrentClipTime() const
		{
			return mClipDuration;
		}

		void SetTime(float time)
		{
			mTimer = time;
		}

		void SetSpeed(float speed)
		{
			mSpeed = speed;
		}

		void StopAnimation(bool s)
		{
			if (s)
			{
				mCurrentTimer = mTimer;
			}
		}

		void StopLoop(bool l)
		{
			mStop = l;
		}

		std::vector<JimmyGod::Math::Matrix4>& GetBoneMatrices() { return mBoneMatrices; }

	private:
		const Model* mModel;
		float mTimer = 0.0f;
		float mFinishTimer = 0.0f;
		float mSpeed = 1.0f;
		float mCurrentTimer = 0.0f;
		float mClipDuration = 0.0f;
		int mClipIndex = 0;

		float mBlendTimer = 0.0f;
		float mBlendWeight = 0.0f;
		float mBlendTime = 0.0f;
		float mBlendDuration = 0.0f;
		int mBlendIndex = 0;

		bool mStop = false;

		std::vector<JimmyGod::Math::Matrix4> mBoneMatrices;
	};
}