#pragma once

#include "Common.h"

namespace JimmyGod::Core
{
	template<class DataType>
	class HandlePool;

	template<class DataType>
	class Handle
	{
	public:
		Handle()
			: mIndex(0) // slot 0 will be unused, in other words handle at 0 is an invalidate handle
			, mGeneration(0)
		{}
		bool IsValid() const
		{
			return sPool && sPool->IsValid(*this);
		}
		void Invalidate()
		{
			*this = Handle();
		}

		DataType* Get() const
		{
			return sPool ? sPool->Get(*this) : nullptr;
		}
		DataType* operator->() const
		{
			return sPool ? sPool->Get(*this) : nullptr;
		}

		DataType& operator*() const
		{
			return *(sPool->Get(*this));
		}

		bool operator== (Handle rhs) const 
		{
			return mIndex == rhs.mIndex &&
				mGeneration == rhs.mGeneration;
		}

		bool operator!= (Handle rhs) const
		{
			return !(*this == rhs);
		}

	private:
		friend class HandlePool<DataType>;
		static HandlePool<DataType>* sPool;
		uint32_t mIndex : 16; // bit packing
		uint32_t mGeneration : 16;
	};

	template<class DataType>
	HandlePool<DataType>* Handle<DataType>::sPool = nullptr;

	// sizeof(Handle) = 4 bytes == sizeof(void*)
}


//// Example
//class Tower
//{
//public:
//	void Update()
//	{
//		Enemy* e = HandlePool.Get(mCureentTarget);
//		if (e)
//		{
//			shootAt(e);
//			if (e->IsDead())
//				mCureentTarget = nullptr;
//		}
//		else
//		{
//			findTarget();
//		}
//	}
//private:
//	Handle mCurrentTarget;
//	Enemy* mCureentTarget --> Dangling pointer
//};

// Example 2
//Handle<Car> myCar;
//myCar->Move();
//std::unique_ptr<Car> cptr;
//cptr->Move();

// Example 3
//class Tower
//{
//public:
//	void Update()
//	{
//		Enemy* enemy = Handle::Instance().Invalidate();
//		Enemy* enemy = mHandlePool.Get(mTarget);
//		if (enemy)
//		{
//			// do stuff
//		}
//	}
//private:
//	Handle<Enemy> mTarget;
//};