#pragma once
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
namespace JimmyCom
{
	class Environment
	{
	public:
		Environment(std::string name, bool destructible);

		void Initialize(JimmyGod::GameWorld* gameWorld, std::filesystem::path& path);

		virtual void Terminate();
		virtual void Update(float deltaTime);
		virtual void Render(const JimmyGod::Graphics::Camera& camera);

		virtual JimmyGod::Math::Vector3& GetPosition() const { return mGameObject->GetComponent<TransformComponent>()->pos; }

		virtual void TakeDamage(float dmg);

		virtual float GetTerritory() const;
		virtual void InstallGrid();
		virtual void Destory() { mActive = false; }

	protected:
		JimmyGod::GameObject* mGameObject = nullptr;

		std::string mName = "";

		bool mActive = false;
		bool mDestructible = false;
		float mHealth = 0.0f;

		struct TransformData
		{
			JimmyGod::Math::Matrix4 world;
			JimmyGod::Math::Matrix4 wvp;
			JimmyGod::Math::Vector3 viewPosition;
			float padding;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;
	};
}