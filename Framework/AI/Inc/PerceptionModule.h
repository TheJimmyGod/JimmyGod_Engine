#pragma once

#include "MemoryRecord.h"
#include "Sensor.h"

namespace JimmyGod::AI
{
	class Agent;

	using ImporatanceCalculator = std::function<void(const Agent&, MemoryRecord&)>;

	//auto func = [32](bool b)-> float { return 4.2f; ... }; // Closure
	/*struct Functor { int i = 32; float operator()(bool b) { return 4.2f; } };*/ // Rambda function is const capture

	class PerceptionModule
	{
	public:
		explicit PerceptionModule(Agent& agent, ImporatanceCalculator calculator);

		template <class SensorType>
		SensorType* AddSensor(std::string name);

		template <class SensorType>
		SensorType* GetSensor(const std::string& name);

		void Update(float deltaTime);

		void SetMemorySpan(float memorySpan) { mMemorySpan = memorySpan; }

		const MemoryRecords& GetMemoryRecords() const { return mMemory; }
	private:
		using SensorMap = std::unordered_map<std::string, std::unique_ptr<Sensor>>;

		Agent& mAgent;
		ImporatanceCalculator mCalculator;
		SensorMap mSensors;
		MemoryRecords mMemory;
		float mMemorySpan = 0.0f; // how long does the agent remember things

	};
	template<class SensorType>
	inline SensorType * PerceptionModule::AddSensor(std::string name)
	{
		//Bad
		//auto[iter, success] = mSensors.try_emplace(std::move(name), std::make_unique<SensorType>);
		//return success ? static_cast<SensorType*>(iter->second.get()) : nullptr;

		//Good
		auto[iter, success] = mSensors.try_emplace(std::move(name), nullptr);
		if (success)
			iter->second = std::make_unique<SensorType>();
		return static_cast<SensorType*>(iter->second.get());
	}
	template<class SensorType>
	inline SensorType * PerceptionModule::GetSensor(const std::string & name)
	{
		auto iter = mSensors.find(name);
		return iter != mSensors.end() ? static_cast<SensorType*>(iter->second.get()):nullptr;
	}
}