#ifndef BLOCK_MEMORY_ALLOCATOR_SIMPLE_STACK_H_
#define BLOCK_MEMORY_ALLOCATOR_SIMPLE_STACK_H_

#include <stddef.h>

#include "block-memory-allocator/interface.h"

/**
 * @brief Type of stored in stack data
 *
 */
typedef size_t stackData_t;

/**
 * @brief Stack structure
 *  This implementation of the stack has a constant size
 */
typedef struct {
  stackData_t* buffer;  ///< Allocated stack buffer
  size_t size;          ///< Stack size
  size_t endIdx;        ///< Index of last element
  iMemory_t iMemory;    ///< Store pointers to memory control functions
} simpleStack_t;

/**
 * @brief Initializing the stack
 *
 * @param aSize - Size of stack
 * @param aIMemory -  Store pointers to memory control functions
 * @return simpleStack_t* - Pointer to a structure of the allocated stack
 */
simpleStack_t* InitSimpleStack(size_t aSize, iMemory_t aIMemory);

/**
 * @brief Passing data on the stack without checks
 *
 * @param aStack - Pointer to the stack
 * @param aData - Passed data
 */
void SimpleStackFastPush(simpleStack_t* aStack, stackData_t aData);

/**
 * @brief Passing data on the stack
 *
 * @param aStack - Pointer to the stack
 * @param aData - Passed data
 * @param aErr - Pointer to the returned error
 */
void SimpleStackPush(simpleStack_t* aStack, stackData_t aData, char* aErr);

/**
 * @brief Extract data from the stack
 *
 * @param aStack - Pointer to the stack
 * @param aErr - Pointer to the returned error
 * @return stackData_t - Extracted data
 */
stackData_t SimpleStackPop(simpleStack_t* aStack, char* aErr);

/**
 * @brief Release allocated stack
 *
 * @param aStack - Pointer to the stack
 * @param aErr - Pointer to the returned error
 */
void FreeSimpleStack(simpleStack_t* aStack, char* aErr);

#endif  // BLOCK_MEMORY_ALLOCATOR_SIMPLE_STACK_H_
