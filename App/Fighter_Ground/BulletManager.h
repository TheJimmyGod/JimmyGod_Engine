#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "Energy.h"
#include "Projectile.h"

namespace FighterGround
{
	class BulletManager
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static BulletManager* Get();

	public:
		void Initialize();
		void Terminate();

		void Update(float deltaTime);
		void Render();

		void Fire(const JimmyGod::Math::Vector2& pos, const JimmyGod::Math::Vector2& vel, size_t unit = 0);
		Energy& GetBullet(size_t index) { return mBullets[index]; }

		Projectile& GetMeleeBullet(size_t index) { return mMeleeBullets[index]; }
		Projectile& GetAirForceBullets(size_t index) { return mAirForceBullets[index]; }
		size_t GetBulletCount() const { return std::size(mBullets); }
		size_t GetMeleeBulletCount() const { return std::size(mMeleeBullets); }
		size_t GetAirBulletCount() const { return std::size(mAirForceBullets); }
	private:
		Energy mBullets[100];
		Projectile mMeleeBullets[100];
		Projectile mAirForceBullets[100];
		int mNextBullet[3];

		size_t mEnergyAbility = 0;
	};


}