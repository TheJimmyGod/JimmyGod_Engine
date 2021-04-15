#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include "EnemyType.h"
namespace FighterGround
{
	class Projectile
	{
	public:
		void Load(FighterGround::EnemyType type = FighterGround::EnemyType::Melee);
		void Unload() 
		{
			mTexture[0] = 0;
			mTexture[1] = 0;
		}
		void Update(float deltaTime);
		void Render();

		void Fire(const JimmyGod::Math::Vector2& pos, const JimmyGod::Math::Vector2& vel);
		void Kill() { mActive = false; }
		bool IsActive() const { return mActive; }

		JimmyGod::Math::Circle GetBoundary() const { return JimmyGod::Math::Circle{ mPos,mRadius }; }
	private:
		JimmyGod::Math::Vector2 mPos = Vector2::Zero;
		JimmyGod::Math::Vector2 mVel = Vector2::Zero;
		JimmyGod::Graphics::TextureId mTexture[2];
		bool mActive = false;
		float mRadius = 10.0f;
		float mAngle = 0.0f;
		FighterGround::EnemyType mType = FighterGround::EnemyType::Melee;
	};
}