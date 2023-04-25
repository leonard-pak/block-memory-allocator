#include "block-memory-allocator/simple-stack.h"

simpleStack_t* InitSimpleStack(size_t aSize, iMemory_t aIMemory) {
  simpleStack_t* stack =
      (simpleStack_t*)aIMemory.CaptureMemory(sizeof(simpleStack_t));
  if (!stack) {
    return NULL;
  }

  stack->size = aSize;
  stack->buffer =
      (stackData_t*)aIMemory.CaptureMemory(aSize * sizeof(stackData_t));
  if (!stack->buffer) {
    aIMemory.FreeMemory(stack);
    return NULL;
  }

  stack->endIdx = 0;
  stack->iMemory = aIMemory;
  return stack;
}

void SimpleStackFastPush(simpleStack_t* aStack, stackData_t data) {
  aStack->buffer[aStack->endIdx++] = data;
}

void SimpleStackPush(simpleStack_t* aStack, stackData_t data, char* err) {
  if (!aStack || aStack->endIdx == aStack->size) {
    *err = 1;
    return;
  }
  aStack->buffer[aStack->endIdx++] = data;
}

stackData_t SimpleStackPop(simpleStack_t* aStack, char* err) {
  if (!aStack || aStack->endIdx == 0) {
    *err = 1;
    return 0;
  }
  return aStack->buffer[--aStack->endIdx];
}

void FreeSimpleStack(simpleStack_t* stack, char* err) {
  if (!stack || !stack->buffer) {
    *err = 1;
    return;
  }
  stack->iMemory.FreeMemory(stack->buffer);
  stack->iMemory.FreeMemory(stack);
}
