#include "block-memory-allocator/block-memory-allocator.h"

pool_t* BlockMallocInitPool(iCtrl_t aIControl) {
  size_t blockCount = BMA_POOL_SIZE / BMA_BLOCK_SIZE;
  size_t alignedPoolSize = BMA_BLOCK_SIZE * blockCount;

  pool_t* pool = (pool_t*)aIControl.Alloc(sizeof(pool_t));
  if (!pool) {
    return NULL;
  }

  iMemory_t iMemory;
  iMemory.Alloc = aIControl.Alloc;
  iMemory.Free = aIControl.Free;

  pool->freeBlockIdxStack = InitSimpleStack(blockCount, iMemory);
  pool->pool = aIControl.Alloc(alignedPoolSize);
  if (!pool->pool) {
    aIControl.Free(pool);
    return NULL;
  }

  pool->poolSize = alignedPoolSize;
  pool->control = aIControl;

  for (size_t i = 0; i < blockCount; ++i) {
    SimpleStackFastPush(pool->freeBlockIdxStack, i);
  }

  return pool;
}

block_t* BlockMallocGetBlock(pool_t* aPool) {
  block_t* newBlock = NULL;
  aPool->control.LockMutex();
  if (!aPool) {
    goto unlock;
  }
  newBlock = (block_t*)aPool->control.Alloc(sizeof(block_t));
  if (!newBlock) {
    goto unlock;
  }

  char err = 0;
  newBlock->idx = SimpleStackPop(aPool->freeBlockIdxStack, &err);
  if (err) {
    aPool->control.Free(newBlock);
    newBlock = NULL;
    goto unlock;
  }

  newBlock->data =
      (blockData_t*)((char*)aPool->pool + newBlock->idx * BMA_BLOCK_SIZE);

unlock:
  aPool->control.UnlockMutex();
  return newBlock;
}

void BlockMallocFreeBlock(pool_t* aPool, block_t* aBlock, char* aErr) {
  aPool->control.LockMutex();
  SimpleStackPush(aPool->freeBlockIdxStack, aBlock->idx, aErr);
  aPool->control.UnlockMutex();
  if (*aErr) {
    return;
  }
  aPool->control.Free(aBlock);
}

void BlockMallocFreePool(pool_t* aPool, char* aErr) {
  if (!aPool || !aPool->pool) {
    *aErr = 1;
    return;
  }

  FreeSimpleStack(aPool->freeBlockIdxStack, aErr);
  if (*aErr) {
    return;
  }

  aPool->control.Free(aPool->pool);
  aPool->control.Free(aPool);
}
