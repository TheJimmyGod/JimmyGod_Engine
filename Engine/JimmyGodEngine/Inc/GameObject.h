#pragma once

namespace JimmyGod
{
	class Component;
	class GameObject;
	class GameWorld;

	using GameObjectAllocator = Core::TypedAllocator<GameObject>;
	using GameObjectHandle = Core::Handle<GameObject>;
	using GameObjectHandlePool = Core::HandlePool<GameObject>;

	class GameObject final
	{
	public:
		GameObject() = default;
		GameObject(const GameObject&) = delete;
		GameObject& operator= (const GameObject&) = delete;
		
		void Initialize();
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		template<class ComponentType>
		ComponentType* AddComponent()
		{
			static_assert(std::is_base_of_v<Component, ComponentType>, "GameObject - Cannot add type ComponentType while");
			ASSERT(!mInitialized, "GameObject -- Cannot add new component");

			auto& newComponent = mComponent.emplace_back(std::make_unique<ComponentType>());
			newComponent->mOwner = this;
			return static_cast<ComponentType*>(newComponent.get());
		}

		template <class ComponentType>
		ComponentType* GetComponent()
		{
			for(aut)

			// HACK - assume the first component is the component we want
			auto iter = mComponent.begin();
			return static_cast<ComponentType*>(iter->get());
		}

		GameWorld& GetWorld() { return *mWorld; }
		const GameWorld& GetWorld() const{ return *mWorld; }

		void SetName(const char* name) { mName = name; }
		const std::string& GetName() const { return mName; }

		GameObjectHandle GetHandle() const { return mHandle; }

	private:
		friend class GameWorld;
		using Components = std::vector<std::unique_ptr<Component>>;
		
		GameWorld* mWorld = nullptr;
		std::string mName = "NoName";
		GameObjectHandle mHandle;
		Components mComponent;
		bool mInitialized = false;
	};
}