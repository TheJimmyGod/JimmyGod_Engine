#pragma once

#include "Unit.h"
#include "CharacterModule.h"


namespace JimmyCom
{
	class Soldier : public CharacterModule, public Unit
	{
	public:
		Soldier(const std::string& name, Flag flag);
		~Soldier() = default;

		Soldier& operator= (const Soldier& rhs) = default;
		Soldier& operator= (Soldier&& rhs);

		AgentComponent& GetAgent() override;
		const AgentComponent& GetAgent() const override;

		virtual const ModelComponent& GetModelComponent() const override;
		virtual ModelComponent& GetModelComponent() override;

		void Initialize(JimmyGod::GameWorld* gameWorld) override;
		void Initialize(JimmyGod::GameWorld* gameWorld, std::filesystem::path& path) override;
		void SetProcess(bool p) override;
		void TakeDamage(float val) override;
		void Reset() override;
	};
}

