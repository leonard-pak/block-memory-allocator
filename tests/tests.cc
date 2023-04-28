extern "C" {
#include "block-memory-allocator/block-memory-allocator.h"
}

#include "gtest/gtest.h"

/**
 * @brief Testing initialization
 *
 */
TEST(BlockMemoryAllocator, TestInitialization) {
  iCtrl_t iCtrl;
  iCtrl.Alloc = malloc;
  iCtrl.Free = free;
  iCtrl.LockMutex = []() {};
  iCtrl.UnlockMutex = []() {};
  pool_t* memoryPool = BlockMallocInitPool(iCtrl);
  ASSERT_NE(memoryPool, nullptr);
  char err = 0;
  BlockMallocFreePool(memoryPool, &err);
  ASSERT_FALSE(err);
}

/**
 * @brief Testing block allocation
 *
 */
TEST(BlockMemoryAllocator, TestSimpleAllocation) {
  iCtrl_t iCtrl;
  iCtrl.Alloc = malloc;
  iCtrl.Free = free;
  iCtrl.LockMutex = []() {};
  iCtrl.UnlockMutex = []() {};
  pool_t* memoryPool = BlockMallocInitPool(iCtrl);
  ASSERT_NE(memoryPool, nullptr);
  auto blockCount = memoryPool->poolSize / BMA_BLOCK_SIZE;
  auto blocks = std::vector<block_t*>();
  for (size_t i = 0; i < blockCount; ++i) {
    blocks.push_back(BlockMallocGetBlock(memoryPool));
    EXPECT_NE(blocks.back(), nullptr);
  }
  char err = 0;
  for (size_t i = 0; i < blockCount; ++i) {
    err = 0;
    BlockMallocFreeBlock(memoryPool, blocks.back(), &err);
    blocks.pop_back();
    EXPECT_FALSE(err);
  }
  err = 0;
  BlockMallocFreePool(memoryPool, &err);
  EXPECT_FALSE(err);
}

/**
 * @brief Testing block allocation after block release
 *
 */
TEST(BlockMemoryAllocator, TestReAllocation) {
  iCtrl_t iCtrl;
  iCtrl.Alloc = malloc;
  iCtrl.Free = free;
  iCtrl.LockMutex = []() {};
  iCtrl.UnlockMutex = []() {};
  pool_t* memoryPool = BlockMallocInitPool(iCtrl);
  ASSERT_NE(memoryPool, nullptr);
  auto blockCount = memoryPool->poolSize / BMA_BLOCK_SIZE;
  auto blocks = std::vector<block_t*>();
  for (size_t i = 0; i < blockCount; ++i) {
    blocks.push_back(BlockMallocGetBlock(memoryPool));
    EXPECT_NE(blocks.back(), nullptr);
  }

  char err = 0;
  BlockMallocFreeBlock(memoryPool, blocks[blockCount / 2], &err);
  EXPECT_FALSE(err);

  auto newBlock = BlockMallocGetBlock(memoryPool);
  EXPECT_NE(newBlock, nullptr);
  blocks[blockCount / 2] = newBlock;

  for (size_t i = 0; i < blockCount; ++i) {
    err = 0;
    BlockMallocFreeBlock(memoryPool, blocks.back(), &err);
    blocks.pop_back();
    EXPECT_FALSE(err);
  }
  err = 0;
  BlockMallocFreePool(memoryPool, &err);
  EXPECT_FALSE(err);
}

/**
 * @brief Testing block allocation after the end of free blocks
 *
 */
TEST(BlockMemoryAllocator, TestOverAllocation) {
  iCtrl_t iCtrl;
  iCtrl.Alloc = malloc;
  iCtrl.Free = free;
  iCtrl.LockMutex = []() {};
  iCtrl.UnlockMutex = []() {};
  pool_t* memoryPool = BlockMallocInitPool(iCtrl);
  ASSERT_NE(memoryPool, nullptr);
  auto blockCount = memoryPool->poolSize / BMA_BLOCK_SIZE;
  auto blocks = std::vector<block_t*>();
  for (size_t i = 0; i < blockCount; ++i) {
    blocks.push_back(BlockMallocGetBlock(memoryPool));
    EXPECT_NE(blocks.back(), nullptr);
  }

  auto newBlock = BlockMallocGetBlock(memoryPool);
  EXPECT_EQ(newBlock, nullptr);

  char err = 0;
  for (size_t i = 0; i < blockCount; ++i) {
    err = 0;
    BlockMallocFreeBlock(memoryPool, blocks.back(), &err);
    blocks.pop_back();
    EXPECT_FALSE(err);
  }
  err = 0;
  BlockMallocFreePool(memoryPool, &err);
  EXPECT_FALSE(err);
}
