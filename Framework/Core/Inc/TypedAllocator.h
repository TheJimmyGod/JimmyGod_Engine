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
			// TODO
			// Get a new block from BlockAllocator
			// Use placement new on the returned block

			DataType* alloc = reinterpret_cast<DataType*>(Allocate());
			if (alloc)
				new (alloc) DataType(std::forward<Args>(args)...);
			return alloc;
		}

		//// Part 2!
		//template<class... Args>
		//DataType* New()
		//{
		//	// TODO
		//	// Modify New() so it is using variadic templete arguments and perfect forwarding
		//}

		void Delete(DataType* ptr)
		{
			// TODO
			// Call destructor on ptr, then Return ptr to BlockAllocator

			if (ptr == nullptr)
				return;

			ptr->~DataType(); // delete ptr;
			Free(ptr);
		}
	};
}