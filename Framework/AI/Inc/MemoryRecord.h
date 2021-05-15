#pragma once

namespace JimmyGod::AI
{
	using Property = std::variant<int, float, JimmyGod::Math::Vector2>;

	struct MemoryRecord
	{
		std::unordered_map<std::string, Property> properties;
		uint32_t uniqueId = 0;
		float lastRecordedTime = 0.0f;
		float importance = 0.0f;

		bool operator==(const MemoryRecord& rhs) const
		{
			return this->uniqueId == rhs.uniqueId;
		}

		friend bool operator==(const MemoryRecord& lhs, const MemoryRecord& rhs)
		{
			return lhs.uniqueId == rhs.uniqueId;
		}
	};

	using MemoryRecords = std::list<MemoryRecord>; // Double linked list

	MemoryRecord& FindOrCreate(MemoryRecords& records, uint32_t uniqueID);

}


// Malloc =
// - typeless
// - supply the size in bytes
// - return void*

// New = 
// - typesafe
// - supply element count
// - return T*
// - Call constructor -> Car car = new Car'()';

// Union has no concpet of destructor and constructor (it means Union doesn't have 'type save') since it calls from C

//class Car
//{
//
//};
//
//void MakeCar()
//{
//	Car car; -> ?? -> [[no_unique_address]]
//}