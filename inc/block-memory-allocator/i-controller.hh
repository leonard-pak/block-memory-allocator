#ifndef BLOCK_MEMORY_ALLOCATOR_I_CONTROLLER_HH_
#define BLOCK_MEMORY_ALLOCATOR_I_CONTROLLER_HH_

#include <cstddef>

namespace bma {
class IController {
 public:
  IController() = default;
  virtual ~IController() = default;

  virtual void LockMutex() = 0;
  virtual void UnlockMutex() = 0;
  virtual void* Malloc(size_t aSize) = 0;
  virtual void Free(void* aPointer) = 0;
};

}  // namespace bma

#endif  // BLOCK_MEMORY_ALLOCATOR_I_CONTROLLER_HH_
