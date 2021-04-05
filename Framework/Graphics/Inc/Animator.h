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

		void SetTime(float time)
		{
			mTimer = time;
		}

		void StopAnimation(bool s)
		{
			if (s)
			{
				mCurrentTimer = mTimer;
			}
		}

		auto& GetBoneMatrices() { return mBoneMatrices; }

	private:
		const Model* mModel;
		float mTimer = 0.0f;
		float mCurrentTimer = 0.0f;
		int mClipIndex = 0;

		float mBlendTimer = 0.0f;
		float mBlendWeight = 0.0f;
		float mBlendTime = 0.0f;
		float mBlendDuration = 0.0f;
		int mBlendIndex = 0;

		std::vector<JimmyGod::Math::Matrix4> mBoneMatrices;
	};
}