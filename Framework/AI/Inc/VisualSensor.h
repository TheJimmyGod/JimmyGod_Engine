#pragma once

#include "Sensor.h"
namespace JimmyGod::AI
{
	class Agent;

	class VisualSensor : public Sensor
	{
	public:
		float viewRange = 300.0f;
		float viewAngle = 45.0f;
		void Update(Agent& agent, MemoryRecords& memory, float deltaTime) override;
	};
}