extern "C" {
#include "block-memory-allocator/block-memory-allocator.h"
}

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(BlockMemoryAllocator, TestInitilization) {
  iCtrl_t iCtrl;
  iCtrl.Alloc = malloc;
  iCtrl.Free = free;
  iCtrl.LockMutex = []() {};
  iCtrl.UnlockMutex = []() {};
  pool_t* memoryPool = InitAllocator(iCtrl);
  ASSERT_NE(memoryPool, nullptr);
  char err = 0;
  FreeAllocator(memoryPool, &err);
  ASSERT_FALSE(err);
}

TEST(BlockMemoryAllocator, TestSimpleAllocation) {
  iCtrl_t iCtrl;
  iCtrl.Alloc = malloc;
  iCtrl.Free = free;
  iCtrl.LockMutex = []() {};
  iCtrl.UnlockMutex = []() {};
  pool_t* memoryPool = InitAllocator(iCtrl);
  ASSERT_NE(memoryPool, nullptr);
  auto blockCount = memoryPool->poolSize / memoryPool->blockSize;
  auto blocks = std::vector<block_t*>();
  for (size_t i = 0; i < blockCount; ++i) {
    blocks.push_back(GetBlock(memoryPool));
    EXPECT_NE(blocks.back(), nullptr);
  }
  char err = 0;
  for (size_t i = 0; i < blockCount; ++i) {
    err = 0;
    FreeBlock(memoryPool, blocks.back(), &err);
    blocks.pop_back();
    EXPECT_FALSE(err);
  }
  err = 0;
  FreeAllocator(memoryPool, &err);
  EXPECT_FALSE(err);
}

TEST(BlockMemoryAllocator, TestReAllocation) {
  iCtrl_t iCtrl;
  iCtrl.Alloc = malloc;
  iCtrl.Free = free;
  iCtrl.LockMutex = []() {};
  iCtrl.UnlockMutex = []() {};
  pool_t* memoryPool = InitAllocator(iCtrl);
  ASSERT_NE(memoryPool, nullptr);
  auto blockCount = memoryPool->poolSize / memoryPool->blockSize;
  auto blocks = std::vector<block_t*>();
  for (size_t i = 0; i < blockCount; ++i) {
    blocks.push_back(GetBlock(memoryPool));
    EXPECT_NE(blocks.back(), nullptr);
  }

  char err = 0;
  FreeBlock(memoryPool, blocks[blockCount / 2], &err);
  EXPECT_FALSE(err);

  auto newBlock = GetBlock(memoryPool);
  EXPECT_NE(newBlock, nullptr);
  blocks[blockCount / 2] = newBlock;

  for (size_t i = 0; i < blockCount; ++i) {
    err = 0;
    FreeBlock(memoryPool, blocks.back(), &err);
    blocks.pop_back();
    EXPECT_FALSE(err);
  }
  err = 0;
  FreeAllocator(memoryPool, &err);
  EXPECT_FALSE(err);
}

TEST(BlockMemoryAllocator, TestOverAllocation) {
  iCtrl_t iCtrl;
  iCtrl.Alloc = malloc;
  iCtrl.Free = free;
  iCtrl.LockMutex = []() {};
  iCtrl.UnlockMutex = []() {};
  pool_t* memoryPool = InitAllocator(iCtrl);
  ASSERT_NE(memoryPool, nullptr);
  auto blockCount = memoryPool->poolSize / memoryPool->blockSize;
  auto blocks = std::vector<block_t*>();
  for (size_t i = 0; i < blockCount; ++i) {
    blocks.push_back(GetBlock(memoryPool));
    EXPECT_NE(blocks.back(), nullptr);
  }

  auto newBlock = GetBlock(memoryPool);
  EXPECT_EQ(newBlock, nullptr);

  char err = 0;
  for (size_t i = 0; i < blockCount; ++i) {
    err = 0;
    FreeBlock(memoryPool, blocks.back(), &err);
    blocks.pop_back();
    EXPECT_FALSE(err);
  }
  err = 0;
  FreeAllocator(memoryPool, &err);
  EXPECT_FALSE(err);
}
