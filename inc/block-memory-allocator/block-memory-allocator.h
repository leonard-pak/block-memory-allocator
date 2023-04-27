#ifndef BLOCK_MEMORY_ALLOCATOR_H_
#define BLOCK_MEMORY_ALLOCATOR_H_

#include <stddef.h>

#include "block-memory-allocator/interface.h"
#include "block-memory-allocator/simple-stack.h"

#ifndef __SIZE_WIDTH__
#endif  // __SIZE_WIDTH__

#ifndef BMA_BLOCK_SIZE
#define BMA_BLOCK_SIZE (__SIZE_WIDTH__ / 8)
#endif  // BMA_BLOCK_SIZE

#ifndef BMA_POOL_SIZE
#define BMA_POOL_SIZE BMA_BLOCK_SIZE
#endif  // BMA_POOL_SIZE

#if BMA_BLOCK_SIZE == 1
typedef char blockData_t;
#elif BMA_BLOCK_SIZE == 2
typedef __UINT16_TYPE__ blockData_t;
#elif BMA_BLOCK_SIZE == 4
typedef __UINT32_TYPE__ blockData_t;
#elif BMA_BLOCK_SIZE == 8
typedef __UINT64_TYPE__ blockData_t;
#else
typedef void blockData_t;
#endif  // Set block data type

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
