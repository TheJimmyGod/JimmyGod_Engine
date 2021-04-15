#include "GameState.h"
#include "Player.h"
#include "BulletManager.h"
#include "UI.h"
#include "HUD.h"
#include "EnemyManager.h"
#include "EnemyType.h"
#include "RoundManager.h"

using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Input;
using namespace FighterGround;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Gray);
	HUD::StaticInitialize();
	Player::StaticInitialize();
	BulletManager::StaticInitialize();
	EnemyManager::StaticInitialize();
	std::map<std::string, size_t> data = { { "Player",0 } };
	UI::StaticInitialize(data);
	RoundManager::StaticInitialize();
}

void GameState::Terminate()
{
	EnemyManager::StaticTerminate();
	UI::StaticTerminate();
	RoundManager::StaticTerminate();
	HUD::StaticTerminate();
	Player::StaticTerminate();
	BulletManager::StaticTerminate();
}

void GameState::Update(float deltaTime)
{
	RoundManager::Get()->Update(deltaTime);
	Player::Get()->Update(deltaTime);
	BulletManager::Get()->Update(deltaTime);
	EnemyManager::Get()->Update(deltaTime);
}

void GameState::Render()
{
	Player::Get()->Render();
	BulletManager::Get()->Render();
	EnemyManager::Get()->Render();
}

void GameState::DebugUI()
{
	UI::Get()->Render();
	HUD::Get()->Render();
}