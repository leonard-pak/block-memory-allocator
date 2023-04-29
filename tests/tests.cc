#include "block-memory-allocator/allocator.hh"

#include <random>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::AnyNumber;
using ::testing::Expectation;
using ::testing::Return;

class MockController : public bma::IController {
 public:
  MOCK_METHOD(void, LockMutex, (), (override));
  MOCK_METHOD(void, UnlockMutex, (), (override));
  MOCK_METHOD(void, Free, (void*), (override));
  MOCK_METHOD(void*, Malloc, (size_t), (override));
};

/**
 * @brief Testing initialization success
 *
 */
TEST(BlockMemoryAllocator, TestInitializationSuccess) {
  void* poolPtr = (void*)0xffff;

  auto mockController = std::make_unique<MockController>();
  Expectation mallocCall =
      EXPECT_CALL(*mockController,
                  Malloc(BMA_BLOCK_SIZE * (BMA_POOL_SIZE / BMA_BLOCK_SIZE)))
          .WillOnce(Return(poolPtr));
  EXPECT_CALL(*mockController, Free(poolPtr)).Times(1).After(mallocCall);
  EXPECT_CALL(*mockController, LockMutex()).Times(0);
  EXPECT_CALL(*mockController, UnlockMutex()).Times(0);

  auto allocator = new bma::Allocator(std::move(mockController));
  delete allocator;

  SUCCEED();
}

/**
 * @brief Testing initialization fail
 *
 */
TEST(BlockMemoryAllocator, TestInitializationFail) {
  void* poolPtr = nullptr;

  auto mockController = std::make_unique<MockController>();
  Expectation mallocCall =
      EXPECT_CALL(*mockController,
                  Malloc(BMA_BLOCK_SIZE * (BMA_POOL_SIZE / BMA_BLOCK_SIZE)))
          .WillOnce(Return(poolPtr));
  EXPECT_CALL(*mockController, Free(poolPtr)).Times(0);
  EXPECT_CALL(*mockController, LockMutex()).Times(0);
  EXPECT_CALL(*mockController, UnlockMutex()).Times(0);

  EXPECT_THROW(bma::Allocator(std::move(mockController)), std::bad_alloc);
}

/**
 * @brief Testing single block allocation
 *
 */
TEST(BlockMemoryAllocator, TestSingleBlockAllocation) {
  void* poolPtr = malloc(BMA_BLOCK_SIZE * (BMA_POOL_SIZE / BMA_BLOCK_SIZE));

  auto mockController = std::make_unique<MockController>();
  EXPECT_CALL(*mockController,
              Malloc(BMA_BLOCK_SIZE * (BMA_POOL_SIZE / BMA_BLOCK_SIZE)))
      .WillOnce(Return(poolPtr));
  EXPECT_CALL(*mockController, Free(poolPtr)).Times(1);
  EXPECT_CALL(*mockController, LockMutex()).Times(2);
  EXPECT_CALL(*mockController, UnlockMutex()).Times(2);

  auto allocator = bma::Allocator(std::move(mockController));
  {
    auto newBlock = allocator.BuildBlock();
    EXPECT_NE(newBlock->kDataPtr, nullptr);
  }

  free(poolPtr);
}

/**
 * @brief Testing all blocks allocation and release
 *
 */
TEST(BlockMemoryAllocator, TestBlocksAllocationRelease) {
  void* poolPtr = malloc(BMA_BLOCK_SIZE * (BMA_POOL_SIZE / BMA_BLOCK_SIZE));

  auto mockController = std::make_unique<MockController>();
  EXPECT_CALL(*mockController,
              Malloc(BMA_BLOCK_SIZE * (BMA_POOL_SIZE / BMA_BLOCK_SIZE)))
      .WillOnce(Return(poolPtr));
  EXPECT_CALL(*mockController, Free(poolPtr)).Times(1);
  EXPECT_CALL(*mockController, LockMutex()).Times(AnyNumber());
  EXPECT_CALL(*mockController, UnlockMutex()).Times(AnyNumber());

  auto allocator = bma::Allocator(std::move(mockController));
  auto const blockCount = bma::Allocator::kPoolSize / BMA_BLOCK_SIZE;

  {
    auto blocks = std::vector<std::unique_ptr<bma::Block>>();
    for (size_t i = 0; i < blockCount; ++i) {
      blocks.push_back(allocator.BuildBlock());
      EXPECT_NE(blocks.back()->kDataPtr, nullptr);
    }
  }

  {
    auto blocks = std::vector<std::unique_ptr<bma::Block>>();
    for (size_t i = 0; i < blockCount; ++i) {
      blocks.push_back(allocator.BuildBlock());
      EXPECT_NE(blocks.back()->kDataPtr, nullptr);
    }
  }

  free(poolPtr);
}

/**
 * @brief Testing allocation of one block after random block release when the
 * pool is fully used
 *
 */
TEST(BlockMemoryAllocator, TestReAllocationSingleBlock) {
  void* poolPtr = malloc(BMA_BLOCK_SIZE * (BMA_POOL_SIZE / BMA_BLOCK_SIZE));

  auto mockController = std::make_unique<MockController>();
  EXPECT_CALL(*mockController,
              Malloc(BMA_BLOCK_SIZE * (BMA_POOL_SIZE / BMA_BLOCK_SIZE)))
      .WillOnce(Return(poolPtr));
  EXPECT_CALL(*mockController, Free(poolPtr)).Times(1);
  EXPECT_CALL(*mockController, LockMutex()).Times(AnyNumber());
  EXPECT_CALL(*mockController, UnlockMutex()).Times(AnyNumber());

  auto allocator = bma::Allocator(std::move(mockController));
  auto const blockCount = bma::Allocator::kPoolSize / BMA_BLOCK_SIZE;

  auto blocks = std::vector<std::unique_ptr<bma::Block>>();
  for (size_t i = 0; i < blockCount; ++i) {
    blocks.push_back(allocator.BuildBlock());
    EXPECT_NE(blocks.back()->kDataPtr, nullptr);
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, blockCount - 1);
  blocks.erase(blocks.begin() + distrib(gen));

  auto newBlock = allocator.BuildBlock();
  EXPECT_NE(newBlock->kDataPtr, nullptr);

  free(poolPtr);
}

/**
 * @brief Testing block allocation when the pool is fully used
 *
 */
TEST(BlockMemoryAllocator, TestOverAllocation) {
  void* poolPtr = malloc(BMA_BLOCK_SIZE * (BMA_POOL_SIZE / BMA_BLOCK_SIZE));

  auto mockController = std::make_unique<MockController>();
  EXPECT_CALL(*mockController,
              Malloc(BMA_BLOCK_SIZE * (BMA_POOL_SIZE / BMA_BLOCK_SIZE)))
      .WillOnce(Return(poolPtr));
  EXPECT_CALL(*mockController, Free(poolPtr)).Times(1);
  EXPECT_CALL(*mockController, LockMutex()).Times(AnyNumber());
  EXPECT_CALL(*mockController, UnlockMutex()).Times(AnyNumber());

  auto allocator = bma::Allocator(std::move(mockController));
  auto const blockCount = bma::Allocator::kPoolSize / BMA_BLOCK_SIZE;

  auto blocks = std::vector<std::unique_ptr<bma::Block>>();
  for (size_t i = 0; i < blockCount; ++i) {
    blocks.push_back(allocator.BuildBlock());
    EXPECT_NE(blocks.back()->kDataPtr, nullptr);
  }

  auto newBlock = allocator.BuildBlock();
  EXPECT_EQ(newBlock->kDataPtr, nullptr);

  free(poolPtr);
}
