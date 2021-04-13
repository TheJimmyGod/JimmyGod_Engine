#include "GameState.h"
#include "Player.h"
#include "BulletManager.h"
#include "UI.h"
#include "HUD.h"
#include "EnemyManager.h"
#include "EnemyType.h"

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
}

void GameState::Terminate()
{
	HUD::StaticTerminate();
	Player::StaticTerminate();
	BulletManager::StaticTerminate();
	EnemyManager::StaticTerminate();
	UI::StaticTerminate();
}

void GameState::Update(float deltaTime)
{
	Player::Get()->Update(deltaTime);
	BulletManager::Get()->Update(deltaTime);
	EnemyManager::Get()->Update(deltaTime);
	
}

void GameState::Render()
{
	UI::Get()->Render();
	Player::Get()->Render();
	BulletManager::Get()->Render();
	EnemyManager::Get()->Render();
	HUD::Get()->Render();
}

void GameState::DebugUI()
{
}