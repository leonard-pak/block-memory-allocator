#include "block-memory-allocator/simple-stack.h"

simpleStack_t* InitSimpleStack(size_t aSize, iMemory_t aIMemory) {
  simpleStack_t* stack = (simpleStack_t*)aIMemory.Alloc(sizeof(simpleStack_t));
  if (!stack) {
    return NULL;
  }

  stack->size = aSize;
  stack->buffer = (stackData_t*)aIMemory.Alloc(aSize * sizeof(stackData_t));
  if (!stack->buffer) {
    aIMemory.Free(stack);
    return NULL;
  }

  stack->endIdx = 0;
  stack->iMemory = aIMemory;
  return stack;
}

void SimpleStackFastPush(simpleStack_t* aStack, stackData_t aData) {
  aStack->buffer[aStack->endIdx++] = aData;
}

void SimpleStackPush(simpleStack_t* aStack, stackData_t aData, char* aErr) {
  if (!aStack || aStack->endIdx == aStack->size) {
    *aErr = 1;
    return;
  }
  aStack->buffer[aStack->endIdx++] = aData;
}

stackData_t SimpleStackPop(simpleStack_t* aStack, char* aErr) {
  if (!aStack || aStack->endIdx == 0) {
    *aErr = 1;
    return 0;
  }
  return aStack->buffer[--aStack->endIdx];
}

void FreeSimpleStack(simpleStack_t* stack, char* aErr) {
  if (!stack || !stack->buffer) {
    *aErr = 1;
    return;
  }
  stack->iMemory.Free(stack->buffer);
  stack->iMemory.Free(stack);
}
