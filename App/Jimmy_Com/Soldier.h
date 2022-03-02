#pragma once

#include "Unit.h"
#include "CharacterModule.h"


namespace JimmyCom
{
	class Soldier : public CharacterModule, public Unit
	{
	public:
		Soldier(const std::string& name, Flag flag, const JimmyGod::Math::Vector3& pos, JimmyGod::GameWorld* gameWorld);
		Soldier(const std::string& name, Flag flag, JimmyGod::Math::Vector3&& pos, JimmyGod::GameWorld* gameWorld);

		~Soldier() = default;

		Soldier& operator= (const Soldier& rhs) = default;
		Soldier& operator= (Soldier&& rhs);

		void TakeDamage(float val) override;
		void Reset() override;
		void Move(const AI::Coord& curr, const AI::Coord& dest) override;
	};
}

