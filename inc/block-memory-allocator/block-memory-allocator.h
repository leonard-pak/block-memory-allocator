#ifndef BLOCK_MEMORY_ALLOCATOR_H_
#define BLOCK_MEMORY_ALLOCATOR_H_

#include <stddef.h>

#include "block-memory-allocator/interface.h"
#include "block-memory-allocator/simple-stack.h"

#ifndef BMA_BLOCK_SIZE
#ifndef __SIZE_WIDTH__
#error Must to set __SIZE_WIDTH__ marco as the platform bit size
#endif  // __SIZE_WIDTH__

#define BMA_BLOCK_SIZE (__SIZE_WIDTH__ / 8)
#endif  // BMA_BLOCK_SIZE

#ifndef BMA_POOL_SIZE
#define BMA_POOL_SIZE BMA_BLOCK_SIZE
#endif  // BMA_POOL_SIZE

/**
 * @brief Type of allocated block data
 *
 */
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

/**
 * @brief Structure of the allocated block
 *
 */
typedef struct {
  blockData_t* data;  ///< Pointer to the allocated block
  size_t idx;         ///< Index in the pool
} block_t;

/**
 * @brief Structure of the initialized pool
 *
 */
typedef struct {
  void* pool;                        ///< Pointer to the allocated pool
  simpleStack_t* freeBlockIdxStack;  ///< Pointer to the stack storing the free
                                     ///< block indices
  size_t poolSize;                   ///< Size of the aligned pool
  iCtrl_t control;                   ///< Store pointers to control functions
} pool_t;

/**
 * @brief Initializing the pool
 *
 * @param aIControl - Store pointers to control functions
 * @return pool_t* - Pointer to a structure of the allocated pool
 */
pool_t* BlockMallocInitPool(iCtrl_t aIControl);

/**
 * @brief Getting one block from the allocated pool
 *
 * @param aPool - Pointer to the allocated pool
 * @return block_t* - Pointer to the block structure
 */
block_t* BlockMallocGetBlock(pool_t* aPool);

/**
 * @brief Releasing the allocated block in the pool
 *
 * @param aPool - Pointer to the allocated pool
 * @param aBlock - - Pointer to the allocated block
 * @param aErr - Pointer to the returned error
 */
void BlockMallocFreeBlock(pool_t* aPool, block_t* aBlock, char* aErr);

/**
 * @brief Releasing the allocated pool
 *
 * @param aPool - Pointer to the allocated pool
 * @param aErr - Pointer to the returned error
 */
void BlockMallocFreePool(pool_t* aPool, char* aErr);

#endif  // BLOCK_MEMORY_ALLOCATOR_H_
