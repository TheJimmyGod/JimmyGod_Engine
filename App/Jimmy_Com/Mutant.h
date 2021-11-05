#pragma once

#include "Unit.h"
#include "CharacterModule.h"

namespace JimmyCom
{
	class Mutant : public CharacterModule, public Unit
	{
	public:
		Mutant(const std::string& name, Flag flag);

		~Mutant() = default;

		Mutant& operator= (const Mutant& rhs) = default;
		Mutant& operator= (Mutant&& rhs) noexcept;

		void Initialize(JimmyGod::GameWorld* gameWorld) override;
		void Initialize(JimmyGod::GameWorld* gameWorld, std::filesystem::path& path) override;
		void TakeDamage(float val) override;
		void Reset() override;
		void Move() override;
	};
}

