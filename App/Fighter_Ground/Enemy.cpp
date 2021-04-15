#include "Enemy.h"
#include "Player.h"
#include "BulletManager.h"
#include "UI.h"
#include "RoundManager.h"

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

void FighterGround::Enemy::Load(FighterGround::EnemyType type)
{
	auto grahpicSystem = JimmyGod::Graphics::GraphicsSystem::Get();
	mType = type;
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
		mHealth = 45.0f;
		mSpeed = 10.0f;
	}
		break;
	default:
		break;
	}
	mDeathTextures[0] = TextureManager::Get()->Load("Death7.png");
	mDeathTextures[1] = TextureManager::Get()->Load("Death6.png");
	mDeathTextures[2] = TextureManager::Get()->Load("Death5.png");
	mDeathTextures[3] = TextureManager::Get()->Load("Death4.png");
	mDeathTextures[4] = TextureManager::Get()->Load("Death3.png");
	mDeathTextures[5] = TextureManager::Get()->Load("Death2.png");
	mDeathTextures[6] = TextureManager::Get()->Load("Death1.png");
	mDeathTextures[7] = TextureManager::Get()->Load("Death7.png");
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
		if (Player::Get()->isFinished() == true) return;
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
		if (mState != AnimationState::EnemyDeath)
		{
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

			CollisionByProjectile();
		}

	}
	if(mTimer >= 0.0f)
		mTimer -= deltaTime;
}

void FighterGround::Enemy::Render()
{
	if (isSpawned == false)
		return;
	switch (mType)
	{
	case FighterGround::EnemyType::Melee:
	{
		switch (mState)
		{
		case FighterGround::AnimationState::Right:
			ProcessingFrame(3, 0.25f, 0);
			SpriteRenderManager::Get()->DrawSprite(mMeleeTextures[mFrame], mPos);
			break;
		case FighterGround::AnimationState::Left:
			ProcessingFrame(3, 0.25f, 3);
			SpriteRenderManager::Get()->DrawSprite(mMeleeTextures[mFrame], mPos);
			break;
		case FighterGround::AnimationState::EnemyDeath:
			ProcessingFrame(8, 0.10f, 0);
			SpriteRenderManager::Get()->DrawSprite(mDeathTextures[mFrame], mPos);
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
			ProcessingFrame(2, 0.25f, 0);
			SpriteRenderManager::Get()->DrawSprite(mBossTextures[mFrame], mPos);
			break;
		case FighterGround::AnimationState::Left:
			ProcessingFrame(2, 0.25f, 2);
			SpriteRenderManager::Get()->DrawSprite(mBossTextures[mFrame], mPos);
			break;
		case FighterGround::AnimationState::EnemyDeath:
			ProcessingFrame(8, 0.20f, 0);
			SpriteRenderManager::Get()->DrawSprite(mDeathTextures[mFrame], mPos);
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

size_t Enemy::ProcessingFrame(size_t count, float duration, int startTexture)
{
	if (mTimer <= 0.0f)
	{
		mTimer = duration;
		mFrame = (mFrame + 1) % count + startTexture;
		if (mState == AnimationState::EnemyDeath)
		{
			if (mDeath != count)
			{
				mDeath++;
			}
			else
			{
				isSpawned = false;
			}
		}

	}
	return size_t();
}

void FighterGround::Enemy::TakeDamage(float dmg)
{
	if (!isDead)
		mHealth -= dmg;
	if (mHealth <= 0.0f)
	{
		if (mType != EnemyType::Air)
			mState = AnimationState::EnemyDeath;
		if (mType == EnemyType::Melee)
		{
			UI::Get()->Update(200);
		}
		else if (mType == FighterGround::EnemyType::Boss)
		{
			UI::Get()->Update(500);
		}
		Kill();
	}
}

void FighterGround::Enemy::Kill()
{
	RoundManager::Get()->mTotalCount--;
	isDead = true;
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