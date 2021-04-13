#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "Enemy.h"
namespace FighterGround
{
	class EnemyManager
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static EnemyManager* Get();
	public:
		void Initialize();
		void Terminate();
		void Update(float deltaTime);
		void Render();

		void Spawn(const JimmyGod::Math::Vector2& position, const JimmyGod::Math::Vector2& velocity, size_t unit = 0);
		Enemy& GetMeleeEnemy(size_t index) { return mMeleeEnemy[index]; }
		size_t GetMeleeEnemyCount() const { return std::size(mMeleeEnemy); }

		Enemy& GetAirEnemy(size_t index) { return mAirEnemy[index]; }
		size_t GetAirEnemyCount() const { return std::size(mAirEnemy); }

		Enemy& GetBossEnemy(size_t index) { return mBossEnemy[index]; }
		size_t GetBossEnemyCount() const { return std::size(mBossEnemy); }

	private:
		Enemy mMeleeEnemy[100];
		Enemy mAirEnemy[100];
		Enemy mBossEnemy[100];
		int mNextEnemy[3];
	};
}