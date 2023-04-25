#include "block-memory-allocator/block-memory-allocator.h"

pool_t* InitAllocator(iCtrl_t aAllocInterface) {
  size_t blockCount = BMA_POOL_SIZE / BMA_BLOCK_SIZE;
  size_t alignedPoolSize = BMA_BLOCK_SIZE * blockCount;

  pool_t* pool = (pool_t*)aAllocInterface.CaptureMemory(sizeof(pool_t));
  if (!pool) {
    return NULL;
  }

  iMemory_t iMemory;
  iMemory.CaptureMemory = aAllocInterface.CaptureMemory;
  iMemory.FreeMemory = aAllocInterface.FreeMemory;

  pool->freeBlockIdxStack = InitSimpleStack(blockCount, iMemory);
  pool->pool = aAllocInterface.CaptureMemory(alignedPoolSize);
  if (!pool->pool) {
    aAllocInterface.FreeMemory(pool);
    return NULL;
  }

  pool->blockSize = BMA_BLOCK_SIZE;
  pool->poolSize = alignedPoolSize;
  pool->interface = aAllocInterface;

  for (size_t i = 0; i < blockCount; ++i) {
    SimpleStackFastPush(pool->freeBlockIdxStack, i);
  }

  return pool;
}

block_t* GetBlock(pool_t* aPool) {
  block_t* newBlock = NULL;
  aPool->interface.LockMutex();
  if (!aPool) {
    goto unlock;
  }
  newBlock = (block_t*)aPool->interface.CaptureMemory(sizeof(block_t));
  if (!newBlock) {
    goto unlock;
  }

  char err = 0;
  newBlock->idx = SimpleStackPop(aPool->freeBlockIdxStack, &err);
  if (err) {
    aPool->interface.FreeMemory(newBlock);
    newBlock = NULL;
    goto unlock;
  }

  newBlock->data =
      (blockData_t*)(aPool->pool + newBlock->idx * aPool->blockSize);

unlock:
  aPool->interface.UnlockMutex();
  return newBlock;
}

void FreeBlock(pool_t* aPool, block_t* aBlock, char* err) {
  aPool->interface.LockMutex();
  SimpleStackPush(aPool->freeBlockIdxStack, aBlock->idx, err);
  aPool->interface.UnlockMutex();
  if (*err) {
    return;
  }
  aPool->interface.FreeMemory(aBlock->data);
  aPool->interface.FreeMemory(aBlock);
}

void FreeAllocator(pool_t* aPool, char* err) {
  if (!aPool || !aPool->pool) {
    *err = 1;
    return;
  }
  aPool->interface.FreeMemory(aPool->pool);
  aPool->interface.FreeMemory(aPool);
}
