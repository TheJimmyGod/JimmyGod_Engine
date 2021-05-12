#include "Precompiled.h"
#include "PerceptionModule.h"

using namespace JimmyGod::AI;

PerceptionModule::PerceptionModule(Agent & agent, ImporatanceCalculator calculator)
	:mAgent(agent), mCalculator(std::move(calculator))
{}

void PerceptionModule::Update(float deltaTime)
{
	for (auto&[name, sensor] : mSensors)
	{
		sensor->Update(mAgent, mMemory, deltaTime);
	}
	// Remove any records older than memory span
	for (auto iter = mMemory.begin(); iter != mMemory.end();)
	{
		if (iter->lastRecordedTime + mMemorySpan < JimmyGod::Core::TimeUtil::GetTime())
		{
			iter = mMemory.erase(iter);
		}
		else
			++iter;
	}

	// Calculator importance on new records
	for (auto& m : mMemory)
	{
		mCalculator(mAgent, m);
	}

	// Sort records by importance
	mMemory.sort([](const MemoryRecord& a, const MemoryRecord& b)
	{
		return a.importance > b.importance;
	}
	);
}
