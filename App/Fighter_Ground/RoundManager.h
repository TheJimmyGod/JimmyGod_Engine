#pragma once

#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
#include <queue>
#include "EnemyManager.h"
#include "UI.h"

namespace FighterGround
{
	class RoundManager
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static RoundManager* Get();
	public:
		void Initialize();
		void Terminate();
		void Update(float deltaTime);
		void SetRound();
		void Enqueue(size_t type, size_t num, float time);
		void Execute();

		size_t GetCountOfActiveEnemies() const;

	private:
		friend class UI;
		friend class Enemy;

		std::queue<std::tuple<size_t,size_t,float>> mList;
		float mTimer = 0.0f;
		float mSpawnTimer = 0.0f;
		float mSpawnTimerValue = 0.0f;

		bool isFinished = false;
		bool isRoundStart = false;
		bool isSpawning = false;

		size_t mRound = 1;
		size_t mCount = 0;
		size_t mTotalCount = 0;
		size_t mLabel;
	};
}