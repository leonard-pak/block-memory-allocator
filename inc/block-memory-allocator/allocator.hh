#ifndef BLOCK_MEMORY_ALLOCATOR_ALLOCATOR_HH_
#define BLOCK_MEMORY_ALLOCATOR_ALLOCATOR_HH_

#include <memory>
#include <stack>

#include "block-memory-allocator/block.hh"
#include "block-memory-allocator/i-controller.hh"

namespace bma {

/**
 * @brief Object of block memory allocator
 *
 */
class Allocator {
 public:
  size_t constexpr static kPoolSize = (BMA_POOL_SIZE / BMA_BLOCK_SIZE) *
                                      BMA_BLOCK_SIZE;  ///< Constant pool size

  /**
   * @brief Construct a new Allocator object
   *
   * @param aController - Interface object of control functions
   */
  Allocator(std::unique_ptr<IController> aController);

  /**
   * @brief Destroy the Allocator object
   *
   */
  ~Allocator();

  /**
   * @brief Getting one block from the allocated pool
   *
   * @return std::unique_ptr<Block> - Pointer to the block object
   */
  std::unique_ptr<Block> BuildBlock();

 private:
  void* mPool = nullptr;  ///< Pointer to the allocated pool

  std::stack<size_t> mFreeBlocksIdxs;  ///< Stack storing the free block indices
  std::unique_ptr<IController>
      mController;  ///< Interface object of control functions
};

}  // namespace bma

#endif  // BLOCK_MEMORY_ALLOCATOR_ALLOCATOR_HH_
