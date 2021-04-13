#include "Player.h"
#include "HUD.h"
#include "BulletManager.h"
using namespace JimmyGod;
using namespace JimmyGod::Math;
using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace FighterGround;

static float timer = 0.0f;
static size_t frame = 0;

namespace
{
	bool CollisionCalculation(const JimmyGod::Math::Circle & left, const JimmyGod::Math::Circle & right)
	{
		float combinedRadius = left.radius + right.radius;
		float radiusSqr = combinedRadius * combinedRadius;
		JimmyGod::Math::Vector2 centerToCenter = left.center - right.center;
		float magnitudeSqr = (centerToCenter.x * centerToCenter.x) + (centerToCenter.y * centerToCenter.y);
		return magnitudeSqr < radiusSqr;
	}

	

	std::unique_ptr<Player> sInstance = nullptr;
}

void Player::StaticInitialize()
{
	sInstance = std::make_unique<Player>();
	sInstance.get()->Load();
}

void Player::StaticTerminate()
{
	sInstance.get()->Unload();
	if (sInstance != nullptr)
		sInstance.reset();
}

Player* Player::Get()
{
	return sInstance.get();
}

void FighterGround::Player::Load()
{
	mPos = Vector2{ 200.0f,150.0f };
	mSpeed = 90.0f;
	mHealth = 2;

	mTextures[0] = TextureManager::Get()->Load("BasicRight.png");
	mTextures[1] = TextureManager::Get()->Load("BasicLeft.png");
	mTextures[2] = TextureManager::Get()->Load("Movement1right.png");
	mTextures[3] = TextureManager::Get()->Load("Movement2right.png");
	mTextures[4] = TextureManager::Get()->Load("Movement1left.png");
	mTextures[5] = TextureManager::Get()->Load("Movement2left.png");
	mTextures[6] = TextureManager::Get()->Load("Zpackright1.png");
	mTextures[7] = TextureManager::Get()->Load("Zpackright2.png");
	mTextures[8] = TextureManager::Get()->Load("Zpackleft1.png");
	mTextures[9] = TextureManager::Get()->Load("Zpackleft2.png");
	mTextures[10] = TextureManager::Get()->Load("Attack1right.png");
	mTextures[11] = TextureManager::Get()->Load("Attack2right.png");
	mTextures[12] = TextureManager::Get()->Load("Attack3right.png");
	mTextures[13] = TextureManager::Get()->Load("Attack4right.png");
	mTextures[14] = TextureManager::Get()->Load("Attack5right.png");
	mTextures[15] = TextureManager::Get()->Load("Attack6right.png");
	mTextures[16] = TextureManager::Get()->Load("Attack1left.png");
	mTextures[17] = TextureManager::Get()->Load("Attack2left.png");
	mTextures[18] = TextureManager::Get()->Load("Attack3left.png");
	mTextures[19] = TextureManager::Get()->Load("Attack4left.png");
	mTextures[20] = TextureManager::Get()->Load("Attack5left.png");
	mTextures[21] = TextureManager::Get()->Load("Attack6left.png");
	mTextures[22] = TextureManager::Get()->Load("Death1Right.png");
	mTextures[23] = TextureManager::Get()->Load("Death2Right.png");
	mTextures[24] = TextureManager::Get()->Load("Death1Left.png");
	mTextures[25] = TextureManager::Get()->Load("Death2Left.png");

}
void FighterGround::Player::Unload()
{

}
void FighterGround::Player::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	auto graphicSystem = GraphicsSystem::Get();
	if (mPos.y < 180.0f)
		mPos.y -= mGravity * deltaTime;
	else if (mPos.y > 450.0f)
		mPos.y = 180.0f;
	
	if (mPos.y < 0.5f)
		mPos.y = 0.5f;

	if (isDead == false)
	{
		if (mPos.y < 180.0f)
			isJump = true;
		else
			isJump = false;
		timer -= deltaTime;
		if (inputSystem->IsKeyDown(KeyCode::A))
		{
			mState = AnimationState::Left;
			if (mPos.x >= 0.0f)
				mPos.x -= mSpeed * deltaTime;
		}
		else if (inputSystem->IsKeyDown(KeyCode::D))
		{
			mState = AnimationState::Right;
			if (mPos.x < static_cast<float>(graphicSystem->GetBackBufferWidth()))
				mPos.x += mSpeed * deltaTime;
		}
		if (inputSystem->IsKeyDown(KeyCode::W))
		{
			if (mState == AnimationState::Right)
			{
				mState = AnimationState::RightJump;
			}
			else if (mState == AnimationState::Left)
			{
				mState = AnimationState::LeftJump;
			}
			mPos.y -= mSpeed * deltaTime;
			
		}

		if (inputSystem->IsKeyPressed(KeyCode::P))
		{
			if (isJump == true)
				return;
			switch (mState)
			{
			case FighterGround::AnimationState::Right:
				mState = AnimationState::RightAttack;
				BulletManager::Get()->Fire(mPos, Vector2{ 250.0f,RandomFloat(-25.0f,25.0f) }, 0);
				break;
			case FighterGround::AnimationState::Left:
				mState = AnimationState::LeftAttack;
				BulletManager::Get()->Fire(mPos, Vector2{ -250.0f,RandomFloat(-25.0f,25.0f) }, 0);
				break;
			case FighterGround::AnimationState::RightAttack:
				BulletManager::Get()->Fire(mPos, Vector2{ 250.0f,RandomFloat(-25.0f,25.0f) }, 0);
				break;
				case FighterGround::AnimationState::LeftAttack:
				BulletManager::Get()->Fire(mPos, Vector2{ -250.0f,RandomFloat(-25.0f,25.0f) }, 0);
				break;
			default:
				break;
			}
		}
		CollisionByProjectile();
		invincibilityTrigger(deltaTime);

	}
	else
	{
		if (mState == AnimationState::Left || mState == AnimationState::LeftJump ||
			mState == AnimationState::LeftAttack || mState == AnimationState::LeftStand)
			mState = AnimationState::LeftDeath;
		else if (mState == AnimationState::Right || mState == AnimationState::RightJump ||
			mState == AnimationState::RightAttack || mState == AnimationState::RightStand)
			mState = AnimationState::RightDeath;
	}
	HUD::Get()->Update(mEnergy, mHealth);
}
void FighterGround::Player::Render()
{
	switch (mState)
	{
	case FighterGround::AnimationState::RightStand:
		SpriteRenderManager::Get()->DrawSprite(mTextures[0], mPos);
		break;
	case FighterGround::AnimationState::LeftStand:
		SpriteRenderManager::Get()->DrawSprite(mTextures[1], mPos);
		break;
	case FighterGround::AnimationState::Right:
		if (!isJump)
		{
			frame = ProcessingFrame(2,4.0f,2);
			SpriteRenderManager::Get()->DrawSprite(mTextures[frame], mPos);
		}
		else
		{
			frame = ProcessingFrame(2, 1.0f, 6);
			SpriteRenderManager::Get()->DrawSprite(mTextures[frame], mPos);
		}
		break;
	case FighterGround::AnimationState::Left:
		if (!isJump)
		{
			frame = ProcessingFrame(2, 4.0f, 4);
			SpriteRenderManager::Get()->DrawSprite(mTextures[frame], mPos);
		}
		else
		{
			frame = ProcessingFrame(2, 1.0f, 8);
			SpriteRenderManager::Get()->DrawSprite(mTextures[frame], mPos);
		}
		break;
	case FighterGround::AnimationState::RightJump:
		frame = ProcessingFrame(2, 1.0f, 6);
		SpriteRenderManager::Get()->DrawSprite(mTextures[frame], mPos);
		break;
	case FighterGround::AnimationState::LeftJump:
		frame = ProcessingFrame(2, 1.0f, 8);
		SpriteRenderManager::Get()->DrawSprite(mTextures[frame], mPos);
		break;
	case FighterGround::AnimationState::RightAttack:
		if (!isJump)
		{
			frame = ProcessingFrame(6, 5.0f, 10);
			SpriteRenderManager::Get()->DrawSprite(mTextures[frame], mPos);
		}
		break;
	case FighterGround::AnimationState::LeftAttack:
		if (!isJump)
		{
			frame = ProcessingFrame(6, 5.0f, 16);
			SpriteRenderManager::Get()->DrawSprite(mTextures[frame], mPos);
		}
		break;
	case FighterGround::AnimationState::RightDeath:
	{
		frame = ProcessingFrame(3, 3.0f, 22);
		SpriteRenderManager::Get()->DrawSprite(mTextures[frame], mPos);
	}
		break;
	case FighterGround::AnimationState::LeftDeath:
	{
		frame = ProcessingFrame(3, 3.0f, 25);
		SpriteRenderManager::Get()->DrawSprite(mTextures[frame], mPos);
	}
		break;
	default:
		break;
	}
}

size_t Player::ProcessingFrame(size_t count, float duration, size_t startTexture)
{
	if (timer <= 0.0f)
	{
		timer = duration;
	}
	frame = (frame + 1) % count + startTexture;
	return frame;
}

void Player::CollisionByProjectile()
{
	for (size_t i = 0; i < BulletManager::Get()->GetMeleeBulletCount(); ++i)
	{
		auto& bullet = BulletManager::Get()->GetMeleeBullet(i);
		if (!bullet.IsActive()) continue;
		if (CollisionCalculation(bullet.GetBoundary(), Player::Get()->GetBoundary()))
		{
			bullet.Kill();
			Player::Get()->TakeDamage(1);
		}
	}
	for (size_t i = 0; i < BulletManager::Get()->GetAirBulletCount(); ++i)
	{
		auto& bullet = BulletManager::Get()->GetAirForceBullets(i);
		if (!bullet.IsActive()) continue;
		if (CollisionCalculation(bullet.GetBoundary(), Player::Get()->GetBoundary()))
		{
			bullet.Kill();
			Player::Get()->TakeDamage(1);
		}
	}
}