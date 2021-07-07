#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Component.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "ModelComponent.h"

using namespace JimmyGod;

GameObject * GameObjectFactory::Create(GameObjectAllocator & allocator, std::filesystem::path templateFileName)
{
	using namespace rapidjson;

	auto newObject = allocator.New();
	if (newObject != nullptr)
	{
		FILE* file = nullptr;
		fopen_s(&file, templateFileName.u8string().c_str(), "r");

		char readBuffer[65536];
		FileReadStream is(file, readBuffer, sizeof(readBuffer));
		Document document;
		document.ParseStream(is);
		assert(document.IsObject());
		if (document.HasMember("GameObject") && document["GameObject"].IsObject())
		{
			auto obj = document["GameObject"].GetObjectW();

			if (obj.HasMember("Components") && obj["Components"].IsObject())
			{
				auto comp = obj["Components"].GetObjectW();

				for (auto& component : comp)
				{
					auto metaClass = Core::Meta::FindMetaClass(component.name.GetString());
					auto newComponent = newObject->AddComponent(metaClass);
					metaClass->Deserialize(newComponent, component.value);
				}
			}
		}
		fclose(file);
	}


	return newObject;
}

void GameObjectFactory::Destroy(GameObjectAllocator & allocator, GameObject * gameObject)
{
	allocator.Delete(gameObject);
}
