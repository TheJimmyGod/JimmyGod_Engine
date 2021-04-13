#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "EnemyType.h"
#include "AnimationState.h"
#include "UI.h"

namespace FighterGround
{
	class Enemy
	{
	public:
		void Load(FighterGround::EnemyType type = FighterGround::EnemyType::Melee);
		void Unload();
		void Update(float deltaTime);
		void Render();

		size_t ProcessingFrame(size_t count, float duration, size_t startTexture);

		const JimmyGod::Math::Vector2& GetPosition() const { return mPos; }

		void TakeDamage(float dmg)
		{
			if (!isDead)
				mHealth -= dmg;
			if (mHealth <= 0.0f)
			{
				if (mType != FighterGround::EnemyType::Air)
					mState = FighterGround::AnimationState::EnemyDeath;
				if (mType == FighterGround::EnemyType::Melee)
				{
					UI::Get()->Update(200);
				}
				else if (mType == FighterGround::EnemyType::Boss)
				{
					UI::Get()->Update(500);
				}
				Kill();
			}
		}

		JimmyGod::Math::Circle GetBoundary() const { return JimmyGod::Math::Circle{ mPos,mRadius }; }

		void Spawn(const JimmyGod::Math::Vector2& pos, const JimmyGod::Math::Vector2& vel)
		{
			mPos = pos;
			mVel = vel;
			isDead = false;
		}
		void Kill() { isDead = true; }
		bool IsNotActive() const { return isDead; }
		void CollisionByProjectile();
	private:

		JimmyGod::Graphics::TextureId mMeleeTextures[13];
		JimmyGod::Graphics::TextureId mAirForceTextures[2];
		JimmyGod::Graphics::TextureId mBossTextures[11];

		float mHealth = 0.0f;
		float mSpeed = 0.0f;
		float mRadius = 20.0f;
		float mAttackRate = 2.0f;
		float mAttackTimer = 0.0f;
		FighterGround::EnemyType mType = FighterGround::EnemyType::Melee;
		JimmyGod::Math::Vector2 mPos = JimmyGod::Math::Vector2::Zero;
		JimmyGod::Math::Vector2 mVel = JimmyGod::Math::Vector2::Zero;

		FighterGround::AnimationState mState = FighterGround::AnimationState::Left;
		bool isDead = true;
	};
}