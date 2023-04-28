#ifndef BLOCK_MEMORY_ALLOCATOR_INTERFACE_H_
#define BLOCK_MEMORY_ALLOCATOR_INTERFACE_H_

#include <stddef.h>

/**
 * @brief Simple procedure interface
 *
 */
typedef void (*voidFunc_t)(void);

/**
 * @brief Interface of the memory allocation function
 *
 * @param aSize - Allocated memory size in bytes
 */
typedef void* (*allocFunc_t)(size_t aSize);

/**
 * @brief Interface of the memory release function
 *
 * @param aPtr - Pointer to allocated memory
 */
typedef void (*freeFunc_t)(void* aPtr);

/**
 * @brief Structure of pool control interface
 *
 */
typedef struct {
  voidFunc_t LockMutex;    ///< Mutex lock function
  voidFunc_t UnlockMutex;  ///< Mutex unlock function
  allocFunc_t Alloc;       ///< Memory allocation function
  freeFunc_t Free;         ///< Memory release function
} iCtrl_t;

/**
 * @brief Structure of memory control interface
 *
 */
typedef struct {
  allocFunc_t Alloc;  ///< Memory allocation function
  freeFunc_t Free;    ///< Memory release function
} iMemory_t;

#endif  // BLOCK_MEMORY_ALLOCATOR_INTERFACE_H_
