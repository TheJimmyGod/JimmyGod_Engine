#pragma once

#include "Common.h"

namespace JimmyGod::AI
{
	class Agent;

	class SteeringBehavior
	{
	public:
		SteeringBehavior() = default;
		virtual ~SteeringBehavior() = default;

		virtual JimmyGod::Math::Vector2 Calculate(Agent& agent) = 0;

		void SetActivateDebugUI(bool active) { mDebugActive = active; }
		bool IsDebugUIActive() const { return mDebugActive; }

		void SetActive(bool active) { mActive = active; }
		bool IsActive() const { return mActive; }

	private:
		bool mActive;
		bool mDebugActive;
	};
}