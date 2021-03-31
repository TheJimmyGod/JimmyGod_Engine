#include "Precompiled.h"
#include "MemoryRecord.h"

using namespace JimmyGod::AI;

MemoryRecord & JimmyGod::AI::FindOrCreate(MemoryRecords & records, uint32_t uniqueID)
{
	auto iter = std::find_if(records.begin(), records.end(), [&](const MemoryRecord& record)
	{
		return record.uniqueId == uniqueID; // Find a record
	});
	if (iter == records.end()) // Records are empty, so create an new record
	{
		records.push_back(MemoryRecord());
		iter = records.end();
		iter--;
	}
	return *iter;
}
