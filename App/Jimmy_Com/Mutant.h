#pragma once

#include "Unit.h"
#include "CharacterModule.h"

namespace JimmyCom
{
	class Mutant : public CharacterModule, public Unit
	{
	public:
		Mutant(std::string name, Flag flag);

		~Mutant() = default;

		Mutant& operator= (const Mutant& rhs) = default;
		Mutant& operator= (Mutant&& rhs) noexcept;

		AgentComponent& GetAgent() override;
		const AgentComponent& GetAgent() const override;

		void Initialize(JimmyGod::GameWorld* gameWorld) override;
		void Initialize(JimmyGod::GameWorld* gameWorld, std::filesystem::path& path) override;
		void SetProcess(bool p) override;
		void TakeDamage(float val) override;
		void Reset() override;
	};
}

