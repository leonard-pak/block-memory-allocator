#ifndef BLOCK_MEMORY_ALLOCATOR_UTILS_HH_
#define BLOCK_MEMORY_ALLOCATOR_UTILS_HH_

#ifndef BMA_BLOCK_SIZE
#ifndef __SIZE_WIDTH__
#error Must to set __SIZE_WIDTH__ marco as the platform bit size
#endif  // __SIZE_WIDTH__

#define BMA_BLOCK_SIZE (__SIZE_WIDTH__ / 8)
#endif  // BMA_BLOCK_SIZE

#ifndef BMA_POOL_SIZE
#define BMA_POOL_SIZE BMA_BLOCK_SIZE
#endif  // BMA_POOL_SIZE

namespace bma {

/**
 * @brief Type of allocated block data
 *
 */
#if BMA_BLOCK_SIZE == 1
typedef char blockData_t;
#elif BMA_BLOCK_SIZE == 2
typedef __UINT16_TYPE__ blockData_t;
#elif BMA_BLOCK_SIZE == 4
typedef __UINT32_TYPE__ blockData_t;
#elif BMA_BLOCK_SIZE == 8
typedef __UINT64_TYPE__ blockData_t;
#else
typedef void blockData_t;
#endif  // Set block data type

}  // namespace bma

#endif  // BLOCK_MEMORY_ALLOCATOR_UTILS_HH_
