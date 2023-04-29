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
 * @brief Testing single block allocation
 *
 */
TEST(BlockMemoryAllocator, TestSingleBlockAllocation) {
  iCtrl_t iCtrl;
  iCtrl.Alloc = malloc;
  iCtrl.Free = free;
  iCtrl.LockMutex = []() {};
  iCtrl.UnlockMutex = []() {};
  pool_t* memoryPool = BlockMallocInitPool(iCtrl);
  ASSERT_NE(memoryPool, nullptr);

  block_t* newBlock = BlockMallocGetBlock(memoryPool);
  EXPECT_NE(newBlock, nullptr);

  char err = 0;
  BlockMallocFreeBlock(memoryPool, newBlock, &err);
  EXPECT_FALSE(err);

  err = 0;
  BlockMallocFreePool(memoryPool, &err);
  EXPECT_FALSE(err);
}

/**
 * @brief Testing all blocks allocation and release
 *
 */
TEST(BlockMemoryAllocator, TestBlocksAllocationRelease) {
  iCtrl_t iCtrl;
  iCtrl.Alloc = malloc;
  iCtrl.Free = free;
  iCtrl.LockMutex = []() {};
  iCtrl.UnlockMutex = []() {};
  pool_t* memoryPool = BlockMallocInitPool(iCtrl);
  ASSERT_NE(memoryPool, nullptr);

  for (int i = 0; i < 2; ++i) {
    auto const blockCount = memoryPool->poolSize / BMA_BLOCK_SIZE;
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
  }

  char err = 0;
  BlockMallocFreePool(memoryPool, &err);
  EXPECT_FALSE(err);
}

/**
 * @brief Testing allocation of one block after random block release when the
 * pool is fully used
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

  auto const blockCount = memoryPool->poolSize / BMA_BLOCK_SIZE;
  auto blocks = std::vector<block_t*>();
  for (size_t i = 0; i < blockCount; ++i) {
    blocks.push_back(BlockMallocGetBlock(memoryPool));
    EXPECT_NE(blocks.back(), nullptr);
  }

  std::srand(std::time(0));
  int randBlockIdx = std::rand() % blockCount;

  char err = 0;
  BlockMallocFreeBlock(memoryPool, blocks[randBlockIdx], &err);
  EXPECT_FALSE(err);

  auto newBlock = BlockMallocGetBlock(memoryPool);
  EXPECT_NE(newBlock, nullptr);
  blocks[randBlockIdx] = newBlock;

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
 * @brief Testing block allocation when the pool is fully used
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

  auto const blockCount = memoryPool->poolSize / BMA_BLOCK_SIZE;
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
