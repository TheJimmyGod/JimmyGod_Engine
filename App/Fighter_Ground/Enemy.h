#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "AnimationState.h"
#include "EnemyType.h"


namespace FighterGround
{
	class Enemy
	{
	public:
		void Load(EnemyType type = EnemyType::Melee);
		void Unload();
		void Update(float deltaTime);
		void Render();

		size_t ProcessingFrame(size_t count, float duration, int startTexture);

		const JimmyGod::Math::Vector2& GetPosition() const { return mPos; }

		void TakeDamage(float dmg);

		JimmyGod::Math::Circle GetBoundary() const { return JimmyGod::Math::Circle{ mPos,mRadius }; }

		void Spawn(const JimmyGod::Math::Vector2& pos, const JimmyGod::Math::Vector2& vel)
		{
			mPos = pos;
			mVel = vel;
			isDead = false;
			isSpawned = true;
			mFrame = 0;
			mTimer = 0.0f;
			mDeath = 0;
		}
		void Kill();
		bool IsNotActive() const { return isDead; }
		void CollisionByProjectile();
	private:

		JimmyGod::Graphics::TextureId mMeleeTextures[13];
		JimmyGod::Graphics::TextureId mAirForceTextures[2];
		JimmyGod::Graphics::TextureId mBossTextures[11];
		JimmyGod::Graphics::TextureId mDeathTextures[8];

		float mHealth = 0.0f;
		float mSpeed = 0.0f;
		float mRadius = 8.0f;
		float mAttackRate = 2.0f;
		float mAttackTimer = 0.0f;

		EnemyType mType = EnemyType::Melee;
		JimmyGod::Math::Vector2 mPos = JimmyGod::Math::Vector2::Zero;
		JimmyGod::Math::Vector2 mVel = JimmyGod::Math::Vector2::Zero;

		AnimationState mState = AnimationState::Left;
		bool isDead = true;
		bool isSpawned = false;
		size_t mFrame = 0;
		float mTimer = 0.0f;
		size_t mDeath = 0;
	};
}