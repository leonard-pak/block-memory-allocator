#ifndef BLOCK_MEMORY_ALLOCATOR_SIMPLE_STACK_H_
#define BLOCK_MEMORY_ALLOCATOR_SIMPLE_STACK_H_

#include <stddef.h>

#include "block-memory-allocator/interface.h"

typedef size_t stackData_t;

typedef struct {
  stackData_t* buffer;
  size_t size;
  size_t endIdx;
  iMemory_t iMemory;
} simpleStack_t;

simpleStack_t* InitSimpleStack(size_t, iMemory_t);
void SimpleStackFastPush(simpleStack_t*, stackData_t);
void SimpleStackPush(simpleStack_t*, stackData_t, char* err);
stackData_t SimpleStackPop(simpleStack_t*, char* err);
void FreeSimpleStack(simpleStack_t*, char* err);

#endif  // BLOCK_MEMORY_ALLOCATOR_SIMPLE_STACK_H_
