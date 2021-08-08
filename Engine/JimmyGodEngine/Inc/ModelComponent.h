#pragma once
#include "Component.h"

namespace JimmyGod
{
	class TransformComponent;

	class ModelComponent final : public Component
	{
	public:
		META_CLASS_DECLARE

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;

		void PlayAnimation(int index) { mAnimator.PlayAnimation(index); }
		void SetAnimationSpeed(float value) { mAnimator.SetSpeed(value); }
		void SetAnimationTime(float value) { mAnimator.SetTime(value); }

		void EnableDebug() { if (!isDebugUI) isDebugUI = true; else isDebugUI = false; }

		JimmyGod::Graphics::Model& GetModel() { return mModel; }
		const JimmyGod::Graphics::Model& GetModel() const { return mModel; }

		auto& GetBoneMatrices() { return mAnimator.GetBoneMatrices(); }
		const JimmyGod::Graphics::Animator& GetAnimator() const { return mAnimator; }

		std::string mPath;
		void Active(bool active) { isActive = active; }
	private:
		JimmyGod::Graphics::Model mModel;
		JimmyGod::Graphics::Animator mAnimator;
		const TransformComponent* mTransformComponent = nullptr;

		std::vector<JimmyGod::Math::Matrix4> mBoneMatrix;
		struct BoneTransformData
		{
			JimmyGod::Math::Matrix4 BoneTransforms[211];
		};

		using BoneTransformBuffer = JimmyGod::Graphics::TypedConstantBuffer<BoneTransformData>;
		BoneTransformBuffer mBoneTransformBuffer;

		bool isDebugUI = false;
		bool isActive = true;
	};
}