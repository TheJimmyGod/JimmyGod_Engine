#include"Precompiled.h"
#include"VisualSensor.h"
#include "MemoryRecord.h"
#include "Agent.h"
#include "AIWorld.h"

using namespace JimmyGod::AI;

void VisualSensor::Update(Agent & agent, MemoryRecords & memory, float deltaTime)
{
	auto neighbors = agent.world.GetNeighborhood({ {agent.Position},agent.Radius }, agent.GetTypeId());
	for (auto& neighbor : neighbors)
	{
		if (JimmyGod::Math::Distance(neighbor->Position, agent.Position) > viewRange)
			continue;
		if (JimmyGod::Math::Dot(JimmyGod::Math::Normalize(neighbor->Position - agent.Position), agent.Heading) < cosf(viewAngle))
			continue;
		if (!agent.world.HasLineOfSight(agent.Position, neighbor->Position))
			continue;
		MemoryRecord& record = FindOrCreate(memory, neighbor->GetUniqueId());
		record.properties["lastSeenPosition"/*Any property name to save specific*/] = neighbor->Position; // Save vector2 for where the entity was
		record.lastRecordedTime = JimmyGod::Core::TimeUtil::GetTime(); // Save current time
	}
}
