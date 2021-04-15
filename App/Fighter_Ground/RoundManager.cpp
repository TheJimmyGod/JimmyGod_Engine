#include "RoundManager.h"

using namespace FighterGround;
using namespace JimmyGod;

namespace
{
	std::unique_ptr<RoundManager> sInstance = nullptr;
}

void RoundManager::StaticInitialize()
{
	sInstance = std::make_unique<RoundManager>();
	sInstance->Initialize();
}

void RoundManager::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

RoundManager * RoundManager::Get()
{
	return sInstance.get();
}

void FighterGround::RoundManager::Initialize()
{
	auto tutorialEnemy = std::tuple<size_t, size_t,float>(0, 4, 15.0f);
	Enqueue(std::get<0>(tutorialEnemy), std::get<1>(tutorialEnemy), std::get<2>(tutorialEnemy));
	mTimer = 6.0f;
}

void FighterGround::RoundManager::Terminate()
{
}

void FighterGround::RoundManager::Update(float deltaTime)
{
	if (!isRoundStart)
	{
		if (!isFinished)
			SetRound();
		if (mTimer > 0.0f)
			mTimer -= deltaTime;
		else
		{
			isRoundStart = true;
			isSpawning = true;
		}
	}
	else
	{
		Execute();
	}

	if (mSpawnTimer > 0.0f)
		mSpawnTimer -= deltaTime;
}

void FighterGround::RoundManager::SetRound()
{
	switch (mRound)
	{
	case 1:
		break;
	case 2:
		Enqueue(1, 1, 3.0f);
		Enqueue(0, 4, 5.0f);
		break;
	case 3:
		Enqueue(1, 1, 3.0f);
		Enqueue(0, 4, 5.0f);
		Enqueue(2, 2, 5.0f);
		break;
	case 4:
		break;
	default:
		break;
	}
	isFinished = true;
}

void FighterGround::RoundManager::Enqueue(size_t type, size_t num, float time)
{
	std::tuple<size_t, size_t,float> newQueue = std::make_tuple(type, num,time);
	mList.push(newQueue);
	if(type != 1)
		mTotalCount += num;
}

void FighterGround::RoundManager::Execute()
{
	if (mList.empty() && mTotalCount == 0 && mRound != 4)
	{
		isSpawning = false;
		isRoundStart = false;
		mRound++;
		isFinished = false;
		mTimer = 6.0f;
		return;
	}

	if (mSpawnTimer > 0.0f)
		return;

	if (isSpawning)
	{
		if (mCount == 0)
		{
			if (mList.empty()) return;
			auto& element = mList.front();
			mList.pop();
			mLabel = std::get<0>(element);
			mCount = std::get<1>(element);
			mSpawnTimerValue = std::get<2>(element);
		}

		float rand = JimmyGod::Math::RandomFloat();
		JimmyGod::Math::Vector2 position = Vector2::Zero;
		if (rand >= 0.5f)
			position = Vector2{ 0.0f,180.0f };
		else
			position = Vector2{ 450.0f,180.0f };
		if (mLabel == 1)
		{
			EnemyManager::Get()->Spawn(Vector2{ position.x,position.y - 100.0f}, Math::Vector2{ 200.0f,200.0f }, mLabel);
		}
		else
		{
			EnemyManager::Get()->Spawn(position, Math::Vector2{ 200.0f,200.0f }, mLabel);
		}
		mSpawnTimer += mSpawnTimerValue;
		mCount--;
	}

}

size_t RoundManager::GetCountOfActiveEnemies() const
{
	size_t count = 0;
	for (size_t i = 0; i < EnemyManager::Get()->GetMeleeEnemyCount(); i++)
	{
		auto& entity = EnemyManager::Get()->GetMeleeEnemy(i);
		if (!entity.IsNotActive())
			count++;
	}

	for (size_t i = 0; i < EnemyManager::Get()->GetBossEnemyCount(); i++)
	{
		auto& entity = EnemyManager::Get()->GetBossEnemy(i);
		if (!entity.IsNotActive())
			count++;
	}

	return count;
}