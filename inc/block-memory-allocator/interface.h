#ifndef BLOCK_MEMORY_ALLOCATOR_INTERFACE_H_
#define BLOCK_MEMORY_ALLOCATOR_INTERFACE_H_

#include <stddef.h>

typedef void (*voidFunc_t)(void);
typedef void* (*allocFunc_t)(size_t);
typedef void* (*alignedAllocFunc_t)(size_t, size_t);
typedef void (*freeFunc_t)(void*);

typedef struct {
  voidFunc_t LockMutex;
  voidFunc_t UnlockMutex;
  alignedAllocFunc_t AlignedAlloc;
  allocFunc_t Alloc;
  freeFunc_t Free;
} iCtrl_t;

typedef struct {
  allocFunc_t Alloc;
  freeFunc_t Free;
} iMemory_t;

#endif  // BLOCK_MEMORY_ALLOCATOR_INTERFACE_H_
