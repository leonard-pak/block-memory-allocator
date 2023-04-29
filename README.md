# Block Memory Allocator

## Tasks

- [x] Allocate memory one block at a time of fixed size from static pool
- [x] The block and pool size must be configurable at project build time
- [x] Adapted to work on Embedded platform of different sizes in a RTOS
- [x] Unit-tests

### Description

C++ language library of block memory allocator with pool.

### Features:

- Abstract class **IController** for external implementation thread-safety malloc/free function and mutex locking/unlocking for using in a RTOS
- Using the **std::stack** to manage free blocks
- Use the **BMA_BLOCK_SIZE** and **BMA_POOL_SIZE** options to set block and pool size during project building
- Internal implementation uses the **\_\_SIZE_WIDTH__** macro to get the platform bitsize
- The size of the allocated pool is equalized by the size of the block to the smaller side of the BMA_POOL_SIZE macro

> For more details see the comments in the header files. You can see an example of use in the tests.

### Makefile

- Running tests
``` bash
make test
make test/valgrind # for starting with valgrind
```
- Build
``` bash
make build
```
