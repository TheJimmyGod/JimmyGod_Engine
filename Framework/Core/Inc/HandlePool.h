#pragma once

#include "Common.h"
#include "DebugUtil.h"

namespace JimmyGod::Core
{
	template<class DataType>
	class Handle;

	template<class DataType>
	class HandlePool
	{
	public:
		using HandleType = Handle<DataType>;

		HandlePool(size_t capacity)
		{
			// TODO
			ASSERT(capacity > 0, "Invaild capacity");
			// allocate capacity + 1 entries (we don't use slot 0)
			mEntries.resize(capacity + 1);
			// add freeslots to mFreeSlots (excluding 0)
			mFreeSlots.reserve(capacity);
			for (size_t i = 1; i <= capacity; ++i)
			{
				mFreeSlots.push_back(i);
			}

			ASSERT(HandleType::sPool == nullptr, "HandlePool -- Cannot have more than one pool of this type!");
			HandleType::sPool = this;

			
		}
		~HandlePool()
		{
			ASSERT(HandleType::sPool == this, "HandlePool -- Something is wrong");
			HandleType::sPool = nullptr;
			mFreeSlots.clear();
			mEntries.clear();
		}

		HandleType Register(DataType* instance)
		{
			// TODO

			ASSERT(instance != nullptr, "HandlePool -- Invalid instance.");
			ASSERT(!mFreeSlots.empty(), "HandlePool -- No more free slots available");
			// Get the next free slot
			size_t indice = mFreeSlots.back();
			mFreeSlots.pop_back();
			mEntries[indice].instance = std::move(instance);

			// Set entry instace pointer
			// Return a handle to this entry (set index and generation)
			HandleType handle;
			handle.mIndex = indice;
			handle.mGeneration = mEntries[indice].generation;
			return handle;
		}
		void Unregister(HandleType handle)
		{
			// TODO
			// Skip if handle is invalid
			if (!IsValid(handle))
				return;
			// Find the entry and increment the generation
			// (this invalidates all existing handles to this slot)
			mEntries[handle.mIndex].generation++;

			// Recycle the slot (add the index back to mFreeSlots)
			mFreeSlots.push_back(handle.mIndex);
			handle.Invalidate();
		}
		void Flush()
		{
			// TODO
			// Loop thought all entries and increment generation ( invalidates all existing handles)
			// Re-add all slot indices to mFreeSlots
			const size_t size = mEntries.size();
			for (size_t i = 0; i < size; ++i)
			{
				mEntries[i].generation++;
				mFreeSlots.push_back(i + 1);
			}
		}

		bool IsValid(HandleType handle) const
		{
			return handle != HandleType() && mEntries[handle.mIndex].generation == handle.mGeneration;
		}
		DataType* Get(HandleType handle)
		{
			return IsValid(handle) ? mEntries[handle.mIndex].instance : nullptr;
		}

	private:
		struct Entry
		{
			DataType* instance = nullptr;
			uint32_t generation = 0;
		};

		std::vector<Entry> mEntries;
		std::vector<size_t> mFreeSlots;
	};
}