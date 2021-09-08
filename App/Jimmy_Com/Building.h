#pragma once

#include "Environment.h"
#include "Flag.h"
#include "Unit.h"

namespace JimmyCom
{
	class Building : public Environment
	{
	public:
		Building(std::string name, bool destructible) : Environment(name, destructible)
		{
			mHealth = 100.0f;
		}

		void Initialize(JimmyGod::GameWorld* gameWorld) override
		{
			ASSERT(gameWorld != nullptr, "The Game World does not exist!");
			gameWorld->Create("../../Assets/Templates/Building.json", mName);
			mGameObject = gameWorld->Find(mName).Get();
		}
	};
}