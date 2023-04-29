#include "block-memory-allocator/block.hh"

namespace bma {

Block::Block(blockData_t* aDataPtr, std::function<void(void)> aOnDelete)
    : kDataPtr(aDataPtr), mOnDelete(aOnDelete) {}

Block::~Block() {
  mOnDelete();
}

}  // namespace bma
