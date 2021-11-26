#pragma once

#include "Unit.h"
#include "CharacterModule.h"

namespace JimmyCom
{
	class Mutant : public CharacterModule, public Unit
	{
	public:
		Mutant(const std::string& name, Flag flag, const JimmyGod::Math::Vector3& pos, JimmyGod::GameWorld* gameWorld);
		Mutant(const std::string& name, Flag flag, JimmyGod::Math::Vector3&& pos, JimmyGod::GameWorld* gameWorld);

		~Mutant() = default;

		Mutant& operator= (const Mutant& rhs) = default;
		Mutant& operator= (Mutant&& rhs) noexcept;

		void TakeDamage(float val) override;
		void Reset() override;
		void Move() override;
	};
}

