#ifndef BLOCK_MEMORY_ALLOCATOR_INTERFACE_H_
#define BLOCK_MEMORY_ALLOCATOR_INTERFACE_H_

#include <stddef.h>

typedef void blockData_t;
typedef void (*voidFunc_t)(void);
typedef void* (*mallocFunc_t)(size_t);
typedef void (*freeFunc_t)(void*);

typedef struct {
  voidFunc_t LockMutex;
  voidFunc_t UnlockMutex;
  mallocFunc_t CaptureMemory;
  freeFunc_t FreeMemory;
} iCtrl_t;

typedef struct {
  mallocFunc_t CaptureMemory;
  freeFunc_t FreeMemory;
} iMemory_t;

#endif  // BLOCK_MEMORY_ALLOCATOR_INTERFACE_H_
