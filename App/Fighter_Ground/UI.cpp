#include "UI.h"
#include "Player.h"
using namespace FighterGround;
using namespace JimmyGod;
using namespace JimmyGod::Graphics;

namespace
{
	std::unique_ptr<UI> sInstance = nullptr;
}

void UI::StaticInitialize(std::map<std::string, size_t> data)
{
	sInstance = std::make_unique<UI>();
	sInstance->Initialize(data);
}

void UI::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

UI * UI::Get()
{
	return sInstance.get();
}

void UI::Initialize(std::map<std::string, size_t> data)
{
	mGameOver = TextureManager::Get()->Load("GameOver.png");
	mBackGround = TextureManager::Get()->Load("BackGround.jpg");

	mLeader.clear();

	std::map<std::string, size_t>::iterator it = data.begin();
	for (; it != data.end(); ++it)
	{
		LeaderBoardItem item = { it->first, it->second };
		mLeader.push_back(item);
	}
}

void UI::Terminate()
{
	mLeader.clear();
}

void UI::Update(size_t score)
{
	mTotalScore += score;
}

void UI::Render()
{
	SpriteRenderManager::Get()->DrawSprite(mBackGround, Vector2{450.0f/2.0f,258.0f/2.0f});

	if (std::size(mLeader) <= 0)
	{
		return;
	}
	float xPos = 350.0f;
	float yPos = 0.0f;
	float textSize = 13.0f;
	// print the header

	if (RoundManager::Get()->isRoundStart == false && RoundManager::Get()->mRound != 4)
	{
		std::string time = std::to_string(static_cast<int>(RoundManager::Get()->mTimer));
		SpriteRenderManager::Get()->DrawScreenText(time.c_str(), (450.0f/2.0f) - 20.0f, 258.0f/2.0f - 20.0f, 50.0f, JimmyGod::Graphics::Colors::White);
	}

	if (RoundManager::Get()->mRound == 4)
	{
		SpriteRenderManager::Get()->DrawScreenText("You win!", (450.0f / 2.0f) - 125.0f, (258.0f / 2.0f) - 50.0f, 50.0f, JimmyGod::Graphics::Colors::White);
	}

	SpriteRenderManager::Get()->DrawScreenText("Score", 350.0f, 0.0f, textSize, JimmyGod::Graphics::Colors::AntiqueWhite); // Vector4(R,G,B,A)
	std::string str = std::to_string(mTotalScore);
	SpriteRenderManager::Get()->DrawScreenText(str.c_str(), 410.0f, 15.0f, 10.0f, JimmyGod::Graphics::Colors::White);
	yPos += 10.0f;
	const char* name = mLeader[0].name.c_str();
	SpriteRenderManager::Get()->DrawScreenText(name, xPos, yPos, textSize, JimmyGod::Graphics::Colors::Aqua);

	if (Player::Get()->isFinished())
	{
		SpriteRenderManager::Get()->DrawSprite(mGameOver, Player::Get()->GetPosition());
		SpriteRenderManager::Get()->DrawScreenText("You lose!", (450.0f / 2.0f) - 125.0f, (258.0f / 2.0f) - 50.0f, 50.0f, JimmyGod::Graphics::Colors::White);
	}
}
