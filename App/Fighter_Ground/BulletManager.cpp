#include "BulletManager.h"
#include "EnemyType.h"
#include "Player.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;
using namespace FighterGround;

namespace
{
	std::unique_ptr<BulletManager> sInstnace = nullptr;
}

void FighterGround::BulletManager::StaticInitialize()
{
	sInstnace = std::make_unique<BulletManager>();
	sInstnace->Initialize();
}
void FighterGround::BulletManager::StaticTerminate()
{
	sInstnace.get()->Terminate();
	sInstnace.reset();
}
BulletManager* FighterGround::BulletManager::Get()
{
	return sInstnace.get();
}

void FighterGround::BulletManager::Initialize()
{
	mNextBullet[0] = 0;
	mNextBullet[1] = 0;
	mNextBullet[2] = 0;

	for (auto& bullet : mBullets)
		bullet.Load();
	for (auto& bullet : mMeleeBullets)
		bullet.Load(FighterGround::EnemyType::Melee);
	for (auto& bullet : mAirForceBullets)
		bullet.Load(FighterGround::EnemyType::Air);
}
void FighterGround::BulletManager::Terminate()
{
	for (auto& bullet : mBullets)
		bullet.Unload();
	for (auto& bullet : mMeleeBullets)
		bullet.Unload();
	for (auto& bullet : mAirForceBullets)
		bullet.Unload();
}

void FighterGround::BulletManager::Update(float deltaTime)
{
	for (auto& bullet : mBullets)
		bullet.Update(deltaTime);
	for (auto& bullet : mMeleeBullets)
		bullet.Update(deltaTime);
	for (auto& bullet : mAirForceBullets)
		bullet.Update(deltaTime);
}
void FighterGround::BulletManager::Render()
{
	for (auto& bullet : mBullets)
		bullet.Render();
	for (auto& bullet : mMeleeBullets)
		bullet.Render();
	for (auto& bullet : mAirForceBullets)
		bullet.Render();
}

void FighterGround::BulletManager::Fire(const JimmyGod::Math::Vector2& pos, const JimmyGod::Math::Vector2& vel, size_t unit)
{
	if (unit == 0)
	{
		if (mEnergyAbility >= 4)
		{
			mBullets[mNextBullet[unit]].Fire(pos, vel, true);
			mNextBullet[unit] = (mNextBullet[unit] + 1) % std::size(mBullets);
			mEnergyAbility = 0;
		}
		else
		{
			mBullets[mNextBullet[unit]].Fire(pos, vel);
			mNextBullet[unit] = (mNextBullet[unit] + 1) % std::size(mBullets);
			mEnergyAbility++;
		}

		Player::Get()->mEnergy = mEnergyAbility;
	}
	else if (unit == 1)
	{
		mMeleeBullets[mNextBullet[unit]].Fire(pos, vel);

		mNextBullet[unit] = (mNextBullet[unit] + 1) % std::size(mMeleeBullets);
	}
	else if (unit == 2)
	{
		mAirForceBullets[mNextBullet[unit]].Fire(pos, vel);

		mNextBullet[unit] = (mNextBullet[unit] + 1) % std::size(mAirForceBullets);
	}
}