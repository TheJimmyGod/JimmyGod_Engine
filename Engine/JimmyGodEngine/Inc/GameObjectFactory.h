#pragma once

#include "GameObject.h"

// Taken Json templates here

namespace JimmyGod::GameObjectFactory
{
	GameObject* Create(GameObjectAllocator& allocator, std::filesystem::path templateFileName);
	void Destroy(GameObjectAllocator& allocator, GameObject* gameObject);
}