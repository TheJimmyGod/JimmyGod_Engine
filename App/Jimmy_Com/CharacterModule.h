#pragma once
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>

namespace JimmyCom
{
	class CharacterModule
	{
	public:
		CharacterModule()
		{
			mModuleActive = true;
			mTransformBuffer.Initialize();
		}
		virtual ~CharacterModule() = default;

		virtual void Initialize(JimmyGod::GameWorld* gameWorld, std::filesystem::path& path) = 0;
		virtual void Initialize(JimmyGod::GameWorld* gameWorld) = 0;
		virtual void Terminate();
		virtual void Update(float deltaTime);
		virtual void Render(const JimmyGod::Graphics::Camera& camera);
	protected:
		struct TransformData
		{
			JimmyGod::Math::Matrix4 world;
			JimmyGod::Math::Matrix4 wvp;
			JimmyGod::Math::Vector3 viewPosition;
			float padding;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		JimmyGod::GameObject* mGameObject = nullptr;
		bool mModuleActive = false;
		bool mAnimationProcess = false;
		float mTime = 0.0f;
	};
}
