#include "Projectile.h"

using namespace FighterGround;
using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void FighterGround::Projectile::Load(FighterGround::EnemyType type)
{
	mType = type;
	if (mType == FighterGround::EnemyType::Air)
		mRadius = 5.0f;
	else if (mType == FighterGround::EnemyType::Melee)
		mRadius = 10.0f;
	mTexture[0] = TextureManager::Get()->Load("eBullet.png");
	mTexture[1] = TextureManager::Get()->Load("eBullet2.png");
}

void FighterGround::Projectile::Update(float deltaTime)
{
	auto graphicSystem = GraphicsSystem::Get();
	if (IsActive())
		mPos += mVel * deltaTime;

	if (mPos.x < -100.0f || mPos.x > static_cast<float>(graphicSystem->GetBackBufferWidth()))
		Kill();
}

void FighterGround::Projectile::Render()
{
	if (IsActive())
	{
		if (mType == FighterGround::EnemyType::Air)
			SpriteRenderManager::Get()->DrawSprite(mTexture[1], mPos);
		else if (mType == FighterGround::EnemyType::Melee)
			SpriteRenderManager::Get()->DrawSprite(mTexture[0], mPos);
	}
}

void FighterGround::Projectile::Fire(const JimmyGod::Math::Vector2 & pos, const JimmyGod::Math::Vector2 & vel)
{
	mPos = pos;
	mVel = vel;
	mActive = true;
}
