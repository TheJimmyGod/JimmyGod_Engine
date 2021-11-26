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

		CharacterModule& operator= (const CharacterModule& rhs) = default;
		CharacterModule& operator= (CharacterModule&& rhs) noexcept
		{
			if (this == &rhs)
				return *this;
			*this = std::move(rhs);
			return *this;
		}

		virtual ~CharacterModule() = default;

		virtual void Terminate();
		virtual void Update(float deltaTime);
		virtual bool AnimateTime(float deltaTime);
		virtual void Render(const JimmyGod::Graphics::Camera& camera);
		virtual void Reset() = 0;

		const CharacterModule& GetCharacterModule() const;
		CharacterModule& GetCharacterModule();

		const TransformComponent& GetTransformComponent() const; 
		TransformComponent& GetTransformComponent(); 

		const ColliderComponent& GetColliderComponent() const;
		ColliderComponent& GetColliderComponent();

		const ModelComponent& GetModelComponent() const;
		ModelComponent& GetModelComponent();

		const AgentComponent& GetAgentComponent() const;
		AgentComponent& GetAgentComponent();

		const JimmyGod::Math::Vector3& GetPosition() const;
		const JimmyGod::Math::Quaternion& GetRotation() const;
		const JimmyGod::Math::Sphere& GetSphereCollider() const;
		bool mAnimationProcess = false;

		float mUpdateTime = 0.0f;
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

		float mTime = 0.0f;
	};
}
