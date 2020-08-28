#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Component.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

using namespace JimmyGod;

GameObject * GameObjectFactory::Create(GameObjectAllocator & allocator, std::filesystem::path templateFileName)
{
	using namespace rapidjson;

	auto newObject = allocator.New();

	FILE* file = nullptr;
	fopen_s(&file, templateFileName.u8string().c_str(), "r");

	char readBuffer[65536];
	FileReadStream is(file, readBuffer, sizeof(readBuffer));
	Document document;
	document.ParseStream(is);
	assert(document.IsObject());
	if (document.HasMember("GameObject"))
	{
		auto obj = document["GameObject"].GetObjectW();
		
		if (obj.HasMember("Components"))
		{
			auto comp = obj["Components"].GetObjectW();

			if (comp.HasMember("TransformComponent"))
			{ 
				// Position
				float pos[3]{0};
				auto trans = comp["TransformComponent"].GetObjectW(); // Transform member
				Value& Pos = trans["Position"]; // Go to Postion on Transform member
				assert(Pos.IsObject());
				int i = 0;
				for (Value::ConstMemberIterator iter = Pos.MemberBegin(); iter != Pos.MemberEnd(); ++iter) 
					// Value& Pos has: x y z(name) with values each
				{
					pos[i] = iter->value.GetFloat(); // pos[0] (x) = iter[0]-> x value
					i++;
				}
				auto transform = newObject->AddComponent<TransformComponent>();
				transform->SetPosition({ pos[0],pos[1],pos[2] });
			}
			if (comp.HasMember("ColliderComponent"))
			{
				auto cen = comp["ColliderComponent"].GetObjectW(); // Collider member
				
				// Center
				float center[3]{0};
				Value& arrayVal = cen["Center"]; // Go to Center on Collider member
				assert(arrayVal.IsObject());
				int i = 0;
				for (Value::ConstMemberIterator iter = arrayVal.MemberBegin();
					iter != arrayVal.MemberEnd(); ++iter)
				{
					center[i] = iter->value.GetFloat();
					i++;
				}

				i = 0;
				// Extend
				float extend[3]{0};
				arrayVal = cen["Extend"];
				assert(arrayVal.IsObject());
				for (Value::ConstMemberIterator iter = arrayVal.MemberBegin();
					iter != arrayVal.MemberEnd(); ++iter)
				{
					extend[i] = iter->value.GetFloat();
					i++;
				}
				auto collider = newObject->AddComponent<ColliderComponent>();
				collider->SetAABB({ {center[0],center[1],center[2]},{extend[0],extend[1],extend[2]} });
			}
		}
	}

	// TODO:
	// Construct the game object based on the data in the template file
	// Read a json file
	// add component as described in the file
	// initialize component with values from the file




	//if (templateFileName == "tallBox")
	//{
	//	// TODO:
	//	// Add TransformComponent class, add setters/getters, and a function to return a Matrix4
	//	// Add ColliderComponent class, add setters/getters, and an AABB, and Render to draw the aabb
	//	auto transform = newObject->AddComponent<TransformComponent>();
	//	transform->SetPosition({ -5.0f,0.0f,0.0f });
	//
	//	auto collider = newObject->AddComponent<ColliderComponent>();
	//	collider->SetAABB({ {0.0f,3.0f,0.0f},{1.0f,3.0f,1.0f} });
	//	
	//}
	//else if (templateFileName == "longBox")
	//{
	//	// TODO:
	//	// Add TransformComponent class, add setters/getters, and a function to return a Matrix4
	//	// Add ColliderComponent class, add setters/getters, and an AABB, and Render to draw the aabb
	//	auto transform = newObject->AddComponent<TransformComponent>();
	//	transform->SetPosition({ 0.0f,0.0f,0.0f });
	//
	//	auto collider = newObject->AddComponent<ColliderComponent>();
	//	collider->SetAABB({ {0.0f,1.0f,0.0f},{1.0f,1.0f,4.0f} });
	//
	//}
	//else if (templateFileName == "fatBox")
	//{
	//	// TODO:
	//	// Add TransformComponent class, add setters/getters, and a function to return a Matrix4
	//	// Add ColliderComponent class, add setters/getters, and an AABB, and Render to draw the aabb
	//	auto transform = newObject->AddComponent<TransformComponent>();
	//	transform->SetPosition({ 5.0f,0.0f,0.0f });
	//
	//	auto collider = newObject->AddComponent<ColliderComponent>();
	//	collider->SetAABB({ {0.0f,1.0f,0.0f},{3.0f,1.0f,1.0f} });
	//
	//}
	return newObject;
}

void GameObjectFactory::Destroy(GameObjectAllocator & allocator, GameObject * gameObject)
{
	allocator.Delete(gameObject);
}
