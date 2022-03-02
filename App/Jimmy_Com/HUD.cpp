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

	auto StartFunc = [this]()
	{
		mGameState = State::GamePlay;
		for (auto& b : mMainMenuButtons)
			b->SetDisplay(false);
	};

	auto EndFunc = [this]()
	{
		mGameState = State::End;
		for (auto& b : mMainMenuButtons)
			b->SetDisplay(false);
	};

	auto button1 = new Button("Health1.png","Health1.png", Vector2(2000.0f, 500.0f), 20.0f);
	button1->AddListener(StandbyFunc);
	mButtons.emplace_back(std::move(button1));

	auto button2 = new Button("Health1.png","Health1.png", Vector2(2000.0f, 450.0f), 20.0f);
	button2->AddListener(MoveFunc);
	mButtons.emplace_back(std::move(button2));

	auto button3 = new Button("Health1.png","Health1.png", Vector2(2000.0f, 400.0f), 20.0f);
	button3->AddListener(AttackFunc);
	mButtons.emplace_back(std::move(button3));

	auto button4 = new Button("Energy5.png","Energy1.png", Vector2(1000.0f, 325.0f), 20.0f);
	button4->AddListener(StartFunc);
	button4->SetDisplay(true);
	mMainMenuButtons.emplace_back(std::move(button4));

	auto button5 = new Button("Energy5.png","Energy1.png", Vector2(1000.0f, 400.0f), 20.0f);
	button5->AddListener(EndFunc);
	button5->SetDisplay(true);
	mMainMenuButtons.emplace_back(std::move(button5));

}

void JimmyCom::HUD::Terminate()
{
	TextureManager::Get()->Clear();
	for (auto& button : mButtons) button.reset();
	for (auto& button : mMainMenuButtons) button.reset();
}

void JimmyCom::HUD::Update(float deltaTime)
{
	if (isDisplay)
	{
		mXaxis = Lerp(mXaxis, 1100.0f, deltaTime * 3.5f);
		SpriteRenderManager::Get()->DrawScreenText("Standby", mXaxis, 500.0f, 15.0f, Colors::White);
		SpriteRenderManager::Get()->DrawScreenText("Move", mXaxis, 450.0f, 15.0f, Colors::White);
		SpriteRenderManager::Get()->DrawScreenText("Attack", mXaxis, 400.0f, 15.0f, Colors::White);
		for (auto& button : mButtons) {
			button->Update(deltaTime);
			button->SetPosition(Lerp(button->GetPosition(), Vector2{ 1100.0f, button->GetPosition().y }, deltaTime * 3.0f
			));
		}
	}

	if (mGameState == State::None)
	{
		for (auto& button : mMainMenuButtons)
			button->Update(deltaTime);
	}
}

void JimmyCom::HUD::Render()
{
	for (auto& button : mButtons) button->Render();
	for (auto& button : mMainMenuButtons) button->Render();
}

void JimmyCom::HUD::DisplayMainMenu()
{
	if (mGameState == State::None)
	{
		SpriteRenderManager::Get()->DrawScreenText("Jimmy-Com", 350.0f, 300.0f, 35.0f, Colors::White);

		SpriteRenderManager::Get()->DrawScreenText("Start", 1050.0f, 325.0f, 25.0f, Colors::White);
		SpriteRenderManager::Get()->DrawScreenText("End", 1050.0f, 400.0f, 25.0f, Colors::White);
	}
}

void JimmyCom::HUD::DisplayAllButtons()
{
	for (auto& button : mButtons) button->SetDisplay(true);
	isDisplay = true;
}

void JimmyCom::HUD::DisappearAllButtons()
{
	for (auto& button : mButtons)
	{
		button->SetDisplay(false);
		button->SetPosition(Vector2{ 2000.0f, button->GetPosition().y });
	}
	mXaxis = 2000.0f;
	isDisplay = false;
}
