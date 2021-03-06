#pragma once

#include "GameObjectFactory.h"
#include "Service.h"

namespace JimmyGod
{
	class GameWorld
	{
	public:
		void Initialize(size_t capacity);
		void Terminate();

		template <class ServiceType>
		ServiceType* AddService()
		{
			static_assert(std::is_base_of_v<Service,ServiceType>, "GameWorld -- 'ServiceType' must be derived from type 'service'");
			ASSERT(!mInitialized, "GameWorld -- Cannot add service after world has already initialized");

			auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
			newService->mWorld = this;
			return static_cast<ServiceType*>(newService.get());

		}

		template <class ServiceType>
		const ServiceType* GetService() const
		{
			for (auto& service : mServices)
			{
				if (service->GetMetaClass() == ServiceType::StaticGetMetaClass())
					return static_cast<const ServiceType*>(service.get());
			}
			return nullptr;
			// HACK - assume the first service is the service we want
			//auto iter = mServices.begin();
			//return static_cast<ServiceType*>(iter->get());
		}

		template <class ServiceType>
		ServiceType* GetService()
		{
			auto constMe = static_cast<const GameWorld*>(this);
			return const_cast<ServiceType*>(constMe->GetService<ServiceType>());
		}

		void LoadLevel(const std::filesystem::path& levelFileName);

		GameObjectHandle Create(const std::filesystem::path& templateFileName, std::string name);
		GameObjectHandle Find(const std::string& name);
		const std::vector<GameObject*>& GetGameObjects() const { return mUpdateList; }
		void Destroy(GameObjectHandle handle);

		void Update(float deltaTime);
		void Render();
		void DebugUI();

	private:
		void DestoryInternal(GameObject* gameObject); // DestoryForRealz
		void ProcessDestroyList();

		using Services = std::vector<std::unique_ptr<Service>>;
		using GameObjectList = std::vector<GameObject*>;

		Services mServices;

		std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
		std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;

		GameObjectList mUpdateList;
		GameObjectList mDestroyList;
		bool mInitialized = false;
		bool mUpdating = false;
	};
}