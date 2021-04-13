#include "EnemyManager.h"

using namespace FighterGround;
using namespace JimmyGod;
using namespace JimmyGod::Math;

namespace
{
	std::unique_ptr<EnemyManager> sInstance = nullptr;
}

void EnemyManager::StaticInitialize()
{
	sInstance = std::make_unique<EnemyManager>();
	sInstance->Initialize();
}

void EnemyManager::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

EnemyManager* EnemyManager::Get() { return sInstance.get(); }

void EnemyManager::Initialize()
{
	mNextEnemy[0] = 0;
	mNextEnemy[1] = 0;
	mNextEnemy[2] = 0;

	for (auto& enemy : mMeleeEnemy)
		enemy.Load(EnemyType::Melee);
	for (auto& enemy : mAirEnemy)
		enemy.Load(EnemyType::Air);
	for (auto& enemy : mBossEnemy)
		enemy.Load(EnemyType::Boss);
}

void EnemyManager::Terminate()
{
	for (auto& enemy : mMeleeEnemy)
		enemy.Unload();
	for (auto& enemy : mAirEnemy)
		enemy.Unload();
	for (auto& enemy : mBossEnemy)
		enemy.Unload();
}

void EnemyManager::Update(float deltaTime)
{
	for (auto& enemy : mMeleeEnemy)
		enemy.Update(deltaTime);
	for (auto& enemy : mAirEnemy)
		enemy.Update(deltaTime);
	for (auto& enemy : mBossEnemy)
		enemy.Update(deltaTime);
}

void EnemyManager::Render()
{
	for (auto& enemy : mMeleeEnemy)
		enemy.Render();
	for (auto& enemy : mAirEnemy)
		enemy.Render();
	for (auto& enemy : mBossEnemy)
		enemy.Render();
}

void EnemyManager::Spawn(const JimmyGod::Math::Vector2 & position, const JimmyGod::Math::Vector2 & velocity, size_t unit)
{
	switch (unit)
	{
	case 0:
		if (mMeleeEnemy[mNextEnemy[unit]].IsNotActive())
			mMeleeEnemy[mNextEnemy[unit]].Spawn(position, velocity);
		mNextEnemy[unit] = (mNextEnemy[unit] + 1) % std::size(mMeleeEnemy);
		break;
	case 1:
		if (mAirEnemy[mNextEnemy[unit]].IsNotActive())
			mAirEnemy[mNextEnemy[unit]].Spawn(position, velocity);
		mNextEnemy[unit] = (mNextEnemy[unit] + 1) % std::size(mAirEnemy);
		break;
	case 2:
		if (mBossEnemy[mNextEnemy[unit]].IsNotActive())
			mBossEnemy[mNextEnemy[unit]].Spawn(position, velocity);
		mNextEnemy[unit] = (mNextEnemy[unit] + 1) % std::size(mBossEnemy);
		break;
	default:
		break;
	}

}
