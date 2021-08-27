#include "../App/Jimmy_Com/HUD.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;

namespace
{
	std::unique_ptr<JimmyCom::HUD> sInstance = nullptr;
}

void JimmyCom::HUD::StaticInitialize()
{
	sInstance = std::make_unique<JimmyCom::HUD>();
	sInstance->Initialize();
}
void JimmyCom::HUD::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}
JimmyCom::HUD* JimmyCom::HUD::Get()
{
	return sInstance.get();
}

void JimmyCom::HUD::Initialize()
{
	// TODO: Create icon texture
	//mIcons[0] = TextureManager::Get()->Load("");
	//mIcons[1] = TextureManager::Get()->Load("");
	//mIcons[2] = TextureManager::Get()->Load("");
	//mIcons[3] = TextureManager::Get()->Load("");
}

void JimmyCom::HUD::Terminate()
{
	TextureManager::Get()->Clear();
}

void JimmyCom::HUD::Render()
{
	//SpriteRenderManager::Get()->DrawSprite();
}

void JimmyCom::HUD::RenderText()
{
	//SpriteRenderManager::Get()->DrawScreenText();
}