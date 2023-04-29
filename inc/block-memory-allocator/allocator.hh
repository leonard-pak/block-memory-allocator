#ifndef BLOCK_MEMORY_ALLOCATOR_ALLOCATOR_HH_
#define BLOCK_MEMORY_ALLOCATOR_ALLOCATOR_HH_

#include <memory>
#include <stack>

#include "block-memory-allocator/block.hh"
#include "block-memory-allocator/i-controller.hh"

namespace bma {

class Allocator {
 public:
  size_t constexpr static kPoolSize =
      (BMA_POOL_SIZE / BMA_BLOCK_SIZE) * BMA_BLOCK_SIZE;

  Allocator(std::unique_ptr<IController>);
  ~Allocator();

  std::unique_ptr<Block> BuildBlock();

 private:
  void* mPool = nullptr;

  std::stack<size_t> mFreeBlocksIdxs;
  std::unique_ptr<IController> mController;
};

}  // namespace bma

#endif  // BLOCK_MEMORY_ALLOCATOR_ALLOCATOR_HH_
