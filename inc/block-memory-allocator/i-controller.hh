#ifndef BLOCK_MEMORY_ALLOCATOR_I_CONTROLLER_HH_
#define BLOCK_MEMORY_ALLOCATOR_I_CONTROLLER_HH_

#include <cstddef>

namespace bma {

/**
 * @brief Controller interface for allocator control
 *
 */
class IController {
 public:
  IController() = default;
  virtual ~IController() = default;

  /**
   * @brief Lock mutex in block mode
   *
   */
  virtual void LockMutex() = 0;

  /**
   * @brief Unlock mutex
   *
   */
  virtual void UnlockMutex() = 0;

  /**
   * @brief Allocate memory
   *  must be thread-safety
   * @param aSize - Allocated memory size in bytes
   * @return void* - Pointer to allocated memory
   */
  virtual void* Malloc(size_t aSize) = 0;

  /**
   * @brief Release memory
   * must be thread-safety
   * @param aPointer - Pointer to allocated memory
   */
  virtual void Free(void* aPointer) = 0;
};

}  // namespace bma

#endif  // BLOCK_MEMORY_ALLOCATOR_I_CONTROLLER_HH_
