#include "../App/Jimmy_Com/HUD.h"
#include "UIManager.h"
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
	auto StandbyFunc = [this]()
	{
		UIManager::Get()->SetOrder(3);
	};

	auto MoveFunc = [this]()
	{
		UIManager::Get()->SetOrder(1);
	};

	auto AttackFunc = [this]()
	{
		UIManager::Get()->SetOrder(2);
	};

	auto button1 = new Button("Health1.png", Vector2(1100.0f, 500.0f), 20.0f);
	button1->AddListener(StandbyFunc);
	mButtons.emplace_back(std::move(button1));

	auto button2 = new Button("Health1.png", Vector2(1100.0f, 450.0f), 20.0f);
	button2->AddListener(MoveFunc);
	mButtons.emplace_back(std::move(button2));

	auto button3 = new Button("Health1.png", Vector2(1100.0f, 400.0f), 20.0f);
	button3->AddListener(AttackFunc);
	mButtons.emplace_back(std::move(button3));

	//mButtons[1] = Button("Health1.png", Vector2(1000.0f, 500.0f), 20.0f);
	//mButtons[2] = Button("Health1.png", Vector2(1000.0f, 500.0f), 20.0f);
	//mButtons[3] = Button("Health1.png", Vector2(1000.0f, 500.0f), 20.0f);
}

void JimmyCom::HUD::Terminate()
{
	TextureManager::Get()->Clear();
	for (auto& button : mButtons) button.reset();
}

void JimmyCom::HUD::Update(float deltaTime)
{
	for (auto& button : mButtons) button->Update(deltaTime);
	if (isDisplay)
	{
		SpriteRenderManager::Get()->DrawScreenText("Standby", 1050.0f, 500.0f, 15.0f, Colors::White);
		SpriteRenderManager::Get()->DrawScreenText("Move", 1050.0f, 450.0f, 15.0f, Colors::White);
		SpriteRenderManager::Get()->DrawScreenText("Attack", 1050.0f, 400.0f, 15.0f, Colors::White);
	}

}

void JimmyCom::HUD::Render()
{
	for (auto& button : mButtons) button->Render();
}

void JimmyCom::HUD::DisplayAllButtons()
{
	for (auto& button : mButtons) button->SetDisplay(true);
	isDisplay = true;
}

void JimmyCom::HUD::DisappearAllButtons()
{
	for (auto& button : mButtons) button->SetDisplay(false);
	isDisplay = false;
}
