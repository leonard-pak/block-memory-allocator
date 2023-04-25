#ifndef BLOCK_MEMORY_ALLOCATOR_H_
#define BLOCK_MEMORY_ALLOCATOR_H_

#ifndef BMA_POOL_SIZE
#define BMA_POOL_SIZE 20
#endif  // POOL_SIZE

#ifndef BMA_BLOCK_SIZE
#define BMA_BLOCK_SIZE 4
#endif  // BLOCK_SIZE

#include <stddef.h>

#include "block-memory-allocator/interface.h"
#include "block-memory-allocator/simple-stack.h"

typedef struct {
  blockData_t* data;
  size_t idx;
} block_t;

typedef struct {
  void* pool;
  simpleStack_t* freeBlockIdxStack;
  size_t blockSize;
  size_t poolSize;
  iCtrl_t interface;
} pool_t;

pool_t* InitAllocator(iCtrl_t);
block_t* GetBlock(pool_t*);
void FreeBlock(pool_t*, block_t*, char*);
void FreeAllocator(pool_t*, char*);

#endif  // BLOCK_MEMORY_ALLOCATOR_H_
