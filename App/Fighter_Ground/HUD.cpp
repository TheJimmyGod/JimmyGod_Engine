#include "HUD.h"

using namespace FighterGround;
using namespace JimmyGod;
using namespace JimmyGod::Graphics;

namespace
{
	std::unique_ptr<HUD> sInstance = nullptr;
}

void HUD::StaticInitialize()
{
	sInstance = std::make_unique<HUD>();
	sInstance->Initialize();
}

void HUD::StaticTerminate()
{
	sInstance.reset();
}

HUD * HUD::Get()
{
	return sInstance.get();
}

void HUD::Initialize()
{
	mEnergy[0] = TextureManager::Get()->Load("Energy1.png");
	mEnergy[1] = TextureManager::Get()->Load("Energy2.png");
	mEnergy[2] = TextureManager::Get()->Load("Energy3.png");
	mEnergy[3] = TextureManager::Get()->Load("Energy4.png");
	mEnergy[4] = TextureManager::Get()->Load("Energy5.png");

	mHealth[0] = TextureManager::Get()->Load("Health1.png");
	mHealth[1] = TextureManager::Get()->Load("Health2.png");
	mHealth[2] = TextureManager::Get()->Load("Health3.png");
}

void HUD::Update(size_t energy, size_t hp)
{
	mEnergyRate = energy;
	mHealthRate = hp;
}

void HUD::Render()
{
	switch (mEnergyRate)
	{
	case 0:
		SpriteRenderManager::Get()->DrawSprite(mEnergy[0], Vector2{ 400.0f,210.0f });
		break;
	case 1:
		SpriteRenderManager::Get()->DrawSprite(mEnergy[1], Vector2{ 400.0f,210.0f });
		break;
	case 2:
		SpriteRenderManager::Get()->DrawSprite(mEnergy[2], Vector2{ 400.0f,210.0f });
		break;
	case 3:
		SpriteRenderManager::Get()->DrawSprite(mEnergy[3], Vector2{ 400.0f,210.0f });
		break;
	case 4:
		SpriteRenderManager::Get()->DrawSprite(mEnergy[4], Vector2{ 400.0f,210.0f });
		break;
	default:
		break;
	}

	switch (mHealthRate)
	{
		case 0:
			break;
	case 1:
		SpriteRenderManager::Get()->DrawSprite(mHealth[0], Vector2(50.0f, 30.0f));
		break;
	case 2:
		SpriteRenderManager::Get()->DrawSprite(mHealth[0], Vector2(50.0f, 30.0f));
		SpriteRenderManager::Get()->DrawSprite(mHealth[1], Vector2(95.0f, 30.0f));
		break;
	case 3:
		SpriteRenderManager::Get()->DrawSprite(mHealth[0], Vector2(50.0f, 30.0f));
		SpriteRenderManager::Get()->DrawSprite(mHealth[1], Vector2(95.0f, 30.0f));
		SpriteRenderManager::Get()->DrawSprite(mHealth[2], Vector2(140.0f, 30.0f));
		break;
	default:
		break;
	}
}
