#pragma once

#include "BlockAllocator.h"

namespace JimmyGod::Core
{
	template<class DataType>
	class TypedAllocator :private BlockAllocator
	{
	public:
		TypedAllocator(size_t capacity)
			: BlockAllocator(sizeof(DataType), capacity)
		{
		}

		template<class... Args>
		DataType* New(Args&&... args)
		{
			DataType* alloc = reinterpret_cast<DataType*>(Allocate());
			if (alloc) new (alloc) DataType(std::forward<Args>(args)...);
			return alloc;
		}

		void Delete(DataType* ptr)
		{
			if (ptr == nullptr) return;
			ptr->~DataType(); // delete ptr;
			Free(ptr);
		}
	};
}