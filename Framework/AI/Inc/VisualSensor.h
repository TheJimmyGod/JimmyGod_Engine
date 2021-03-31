#pragma once

#include "Sensor.h"
namespace JimmyGod::AI
{
	class Agent;

	class VisualSensor : public Sensor
	{
		float viewRange = 100.0f;
		float viewAngle = 45.0f;

	public:
		void Update(Agent& agent, MemoryRecords& memory, float deltaTime) override;
	};
}