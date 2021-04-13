#include "Energy.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;
using namespace FighterGround;

void FighterGround::Energy::Load()
{
	mTexture[0] = TextureManager::Get()->Load("Missile1.png");
	mTexture[1] = TextureManager::Get()->Load("Missile2.png");
}
void FighterGround::Energy::Unload()
{
	mTexture[0] = 0;
	mTexture[1] = 0;
}
void FighterGround::Energy::Update(float deltaTime)
{
	auto graphicSystem = GraphicsSystem::Get();
	if (IsActive())
		mPos += mVel * deltaTime;

	if (mPos.x < -100.0f || mPos.x > static_cast<float>(graphicSystem->GetBackBufferWidth()))
		Kill();
}
void FighterGround::Energy::Render()
{
	if (IsActive())
		if (mAbility)
			SpriteRenderManager::Get()->DrawSprite(mTexture[1], mPos);
		else
			SpriteRenderManager::Get()->DrawSprite(mTexture[0], mPos);

}

void FighterGround::Energy::Fire(const JimmyGod::Math::Vector2& pos, const JimmyGod::Math::Vector2& vel, bool ability)
{
	mPos = pos;
	mVel = vel;
	mActive = true;
	mAbility = ability;
}

void FighterGround::Energy::Kill()
{
	mActive = false;
}