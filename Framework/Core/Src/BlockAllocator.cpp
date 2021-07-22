#include "Precompiled.h"
#include "BlockAllocator.h"

#include "DebugUtil.h"

using namespace JimmyGod;
using namespace JimmyGod::Core;

BlockAllocator::BlockAllocator(size_t blockSize, size_t capacity)
	: mData(nullptr), mBlockSize(blockSize), mCapacity(capacity)
{
	ASSERT(capacity > 0, "BlockAllocator -- Invaild capacity.");
	mFreeSlots.clear();
	mFreeSlots.reserve(mCapacity);
	mData = reinterpret_cast<uint8_t*>(malloc(mCapacity*mBlockSize));
	for (size_t i = 0; i < mCapacity; i++){ mFreeSlots.push_back(i); }
}

BlockAllocator::~BlockAllocator()
{
	free(mData);
	mData = nullptr;
	mFreeSlots.clear();
}

void * BlockAllocator::Allocate()
{
	if (mFreeSlots.empty()) return nullptr;
	int indice = mFreeSlots.back();
	void* newData = reinterpret_cast<uint8_t*>(mData) + indice * mBlockSize; 
	// ¤¤-> int* num = 1000; num += 1; == 1004
	mFreeSlots.pop_back();
	return newData;
}

void BlockAllocator::Free(void * ptr)
{
	if (!ptr) return;
	auto backSlot = sizeof(&ptr - &mData) / mBlockSize;
	mFreeSlots.push_back(backSlot);
}
