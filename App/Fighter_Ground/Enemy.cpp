#include "Enemy.h"
#include "Player.h"
#include "BulletManager.h"
#include "UI.h"
using namespace FighterGround;

using namespace JimmyGod;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

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
}

static size_t frame = 0;
static float timer = 0.0f;

void FighterGround::Enemy::Load(FighterGround::EnemyType type)
{
	auto grahpicSystem = JimmyGod::Graphics::GraphicsSystem::Get();
	mType = type;
	float rand = JimmyGod::Math::RandomFloat();
	if (rand >= 0.5f)
		mPos = Vector2{ 0.0f,110.0f };
	else
		mPos = Vector2{ static_cast<float>(grahpicSystem->GetBackBufferWidth()),110.0f };
	switch (type)
	{
	case FighterGround::EnemyType::Melee:
	{
		mMeleeTextures[0] = TextureManager::Get()->Load("WalkingRight.png");
		mMeleeTextures[1] = TextureManager::Get()->Load("WalkingRight2.png");
		mMeleeTextures[2] = TextureManager::Get()->Load("WalkingRight3.png");
		mMeleeTextures[3] = TextureManager::Get()->Load("WalkingLeft.png");
		mMeleeTextures[4] = TextureManager::Get()->Load("WalkingLeft2.png");
		mMeleeTextures[5] = TextureManager::Get()->Load("WalkingLeft3.png");
		mMeleeTextures[6] = TextureManager::Get()->Load("Death1.png");
		mMeleeTextures[7] = TextureManager::Get()->Load("Death2.png");
		mMeleeTextures[8] = TextureManager::Get()->Load("Death3.png");
		mMeleeTextures[9] = TextureManager::Get()->Load("Death4.png");
		mMeleeTextures[10] = TextureManager::Get()->Load("Death5.png");
		mMeleeTextures[11] = TextureManager::Get()->Load("Death6.png");
		mMeleeTextures[12] = TextureManager::Get()->Load("Death7.png");

		mHealth = 12.0f;
		mSpeed = 30.0f;
	}
		break;
	case FighterGround::EnemyType::Air:
	{
		mAirForceTextures[0] = TextureManager::Get()->Load("MonsterRight.png");
		mAirForceTextures[1] = TextureManager::Get()->Load("MonsterLeft.png");
		mHealth = 1.0f;
		mSpeed = 20.0f;
		mAttackRate = 5.0f;
	}
		break;
	case FighterGround::EnemyType::Boss:
	{
		mBossTextures[0] = TextureManager::Get()->Load("RightMove1.png");
		mBossTextures[1] = TextureManager::Get()->Load("RightMove2.png");
		mBossTextures[2] = TextureManager::Get()->Load("LeftMove1.png");
		mBossTextures[3] = TextureManager::Get()->Load("LeftMove2.png");
		mBossTextures[4] = TextureManager::Get()->Load("Death1.png");
		mBossTextures[5] = TextureManager::Get()->Load("Death2.png");
		mBossTextures[6] = TextureManager::Get()->Load("Death3.png");
		mBossTextures[7] = TextureManager::Get()->Load("Death4.png");
		mBossTextures[8] = TextureManager::Get()->Load("Death5.png");
		mBossTextures[9] = TextureManager::Get()->Load("Death6.png");
		mBossTextures[10] = TextureManager::Get()->Load("Death7.png");
		mHealth = 45.0f;
		mSpeed = 10.0f;
	}
		break;
	default:
		break;
	}


}

void FighterGround::Enemy::Unload()
{
}

void FighterGround::Enemy::Update(float deltaTime)
{
	JimmyGod::Math::Vector2 direction = Normalize(Player::Get()->GetPosition() - GetPosition());
	mVel = direction * 150.0f;

	if (!isDead)
	{
		if (mPos.x <= Player::Get()->GetPosition().x)
		{
			mState = FighterGround::AnimationState::Right;
			mPos.x += mSpeed * deltaTime;
		}
		else
		{
			mState = FighterGround::AnimationState::Left;
			mPos.x -= mSpeed * deltaTime;
		}

		if (mType == FighterGround::EnemyType::Boss)
		{
			if (CollisionCalculation(Player::Get()->GetBoundary(), GetBoundary()))
			{
				Player::Get()->TakeDamage(1);
			}
		}

		mAttackTimer -= deltaTime;
		if (mAttackTimer <= 0.0f)
		{
			switch (mType)
			{
			case FighterGround::EnemyType::Melee:
				BulletManager::Get()->Fire(GetPosition(), mVel, 1);
				break;
			case FighterGround::EnemyType::Air:
				BulletManager::Get()->Fire(GetPosition(), mVel, 2);
				break;
			default:
				break;
			}
			mAttackTimer = mAttackRate;
		}
	}
	timer -= deltaTime;
}

void FighterGround::Enemy::Render()
{
	if (isDead)
	{
		return;
	}
	switch (mType)
	{
	case FighterGround::EnemyType::Melee:
	{
		switch (mState)
		{
		case FighterGround::AnimationState::Right:
			frame = ProcessingFrame(3, 0.25f, 0);
			SpriteRenderManager::Get()->DrawSprite(mMeleeTextures[frame], mPos);
			break;
		case FighterGround::AnimationState::Left:
			frame = ProcessingFrame(3, 0.25f, 3);
			SpriteRenderManager::Get()->DrawSprite(mMeleeTextures[frame], mPos);
			break;
		case FighterGround::AnimationState::EnemyDeath:
			frame = ProcessingFrame(7, 0.15f, 6);
			SpriteRenderManager::Get()->DrawSprite(mMeleeTextures[frame], mPos);
			break;
		default:
			break;
		}
	}
	break;
	case FighterGround::EnemyType::Air:
	{
		switch (mState)
		{
		case FighterGround::AnimationState::Right:
			SpriteRenderManager::Get()->DrawSprite(mAirForceTextures[0], mPos);
			break;
		case FighterGround::AnimationState::Left:
			SpriteRenderManager::Get()->DrawSprite(mAirForceTextures[1], mPos);
			break;
		default:
			break;
		}
	}
	break;
	case FighterGround::EnemyType::Boss:
	{
		switch (mState)
		{
		case FighterGround::AnimationState::Right:
			frame = ProcessingFrame(2, 0.25f, 0);
			SpriteRenderManager::Get()->DrawSprite(mMeleeTextures[frame], mPos);
			break;
		case FighterGround::AnimationState::Left:
			frame = ProcessingFrame(2, 0.25f, 2);
			SpriteRenderManager::Get()->DrawSprite(mMeleeTextures[frame], mPos);
			break;
		case FighterGround::AnimationState::EnemyDeath:
			frame = ProcessingFrame(7, 0.15f, 4);
			SpriteRenderManager::Get()->DrawSprite(mMeleeTextures[frame], mPos);
			break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}
}

size_t Enemy::ProcessingFrame(size_t count, float duration, size_t startTexture)
{
	if (timer <= 0.0f)
	{
		timer = duration;
	}
	frame = (frame + 1) % count + startTexture;
	return frame;
}

void Enemy::CollisionByProjectile()
{
	for (size_t i = 0; i < BulletManager::Get()->GetBulletCount(); ++i)
	{
		auto& bullet = BulletManager::Get()->GetBullet(i);
		if (!bullet.IsActive()) continue;
		if (CollisionCalculation(bullet.GetBoundary(), GetBoundary()))
		{
			if (bullet.IsAbility())
			{
				if (mType == FighterGround::EnemyType::Boss)
				{
					TakeDamage(2);
				}
				else
					TakeDamage(4);
			}
			else
				TakeDamage(1);
			bullet.Kill();
		}
	}
}