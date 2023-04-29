#ifndef BLOCK_MEMORY_ALLOCATOR_BLOCK_HH_
#define BLOCK_MEMORY_ALLOCATOR_BLOCK_HH_

#include <functional>

#include "block-memory-allocator/utils.hh"

namespace bma {

/**
 * @brief Object of allocated block
 * Data Transfer Object
 */
struct Block {
 public:
  /**
   * @brief Construct a new Block object
   *
   * @param aDataPtr - Pointer to the allocated block
   * @param aOnDelete - Callback function called in the destructor
   */
  Block(blockData_t* aDataPtr, std::function<void(void)> aOnDelete);

  /**
   * @brief Destroy the Block object
   *
   */
  ~Block();

  const blockData_t* kDataPtr = nullptr;  ///< Pointer to the allocated block

 private:
  std::function<void(void)>
      mOnDelete;  ///< Callback function called in the destructor
};

}  // namespace bma

#endif  // BLOCK_MEMORY_ALLOCATOR_BLOCK_HH_
