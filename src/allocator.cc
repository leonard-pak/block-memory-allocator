#include "block-memory-allocator/allocator.hh"

#include "stdexcept"

namespace bma {

Allocator::Allocator(std::unique_ptr<IController> aController)
    : mController(std::move(aController)) {
  mPool = mController->Malloc(kPoolSize);
  if (!mPool) {
    throw std::bad_alloc();
  }
  for (size_t i = 0; i < kPoolSize / BMA_BLOCK_SIZE; ++i) {
    mFreeBlocksIdxs.push(i);
  }
}
Allocator::~Allocator() {
  mController->Free(mPool);
}

std::unique_ptr<Block> Allocator::BuildBlock() {
  mController->LockMutex();

  if (mFreeBlocksIdxs.empty()) {
    mController->UnlockMutex();
    return std::make_unique<Block>(nullptr, []() {});
  }

  auto blockIdx = mFreeBlocksIdxs.top();
  mFreeBlocksIdxs.pop();

  auto newBlock = std::make_unique<Block>(
      (blockData_t*)((char*)mPool + blockIdx * BMA_BLOCK_SIZE),
      [this, blockIdx]() {
        mController->LockMutex();
        mFreeBlocksIdxs.push(blockIdx);
        mController->UnlockMutex();
      });

  mController->UnlockMutex();
  return newBlock;
}

}  // namespace bma
