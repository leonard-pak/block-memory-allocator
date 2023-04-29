#ifndef BLOCK_MEMORY_ALLOCATOR_BLOCK_HH_
#define BLOCK_MEMORY_ALLOCATOR_BLOCK_HH_

#include <functional>

#include "block-memory-allocator/utils.hh"

namespace bma {

class Block {
 public:
  Block(blockData_t* aDataPtr, std::function<void(void)> aOnDelete);
  ~Block();

  const blockData_t* kDataPtr = nullptr;

 private:
  std::function<void(void)> mOnDelete;
};

}  // namespace bma

#endif  // BLOCK_MEMORY_ALLOCATOR_BLOCK_HH_
