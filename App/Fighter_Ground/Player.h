#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "AnimationState.h"
namespace FighterGround
{
	class Player
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static Player* Get();

	public:
		void Load();
		void Unload();
		void Update(float deltaTime);
		void Render();

		size_t ProcessingFrame(size_t count, float duration, size_t startTexture);
		void CollisionByProjectile();
	public:
		const JimmyGod::Math::Vector2& GetPosition() const { return mPos; }
		JimmyGod::Math::Circle GetBoundary() const { return JimmyGod::Math::Circle{ mPos,20.0f }; }
		
		void TakeDamage(size_t dmg)
		{
			if (isDamaged) return;
			if (!isDead)
			{
				isDamaged = true;
				mTimer = 3.0f;
				mHealth -= dmg;
			}
			else
				isDead = true;
		}

		void invincibilityTrigger(float deltaTime)
		{
			if (mTimer > 0.0f)
				mTimer -= deltaTime;
			else
				isDamaged = false;
		}

		bool isFinished() { return isDead; }

	private:
		JimmyGod::Graphics::TextureId mTextures[28];

		AnimationState mState = AnimationState::LeftStand;
		bool isJump = false;
		bool isDead = false;
		bool isDamaged = false;

	private:
		friend class BulletManager;

		Math::Vector2 mPos = Vector2::One;
		float mHitFrame{2.2f};
		float mSpeed{ 0.0f };
		float mDamage{ 0.0f };
		size_t mHealth{ 0 };
		size_t mEnergy{ 0 };
		float mTimer{ 0.0f };
		const float mGravity{ -40.0f };
	};
}