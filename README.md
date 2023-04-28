# Block Memory Allocator

## Tasks

- [x] Allocate memory one block at a time of fixed size from static pool
- [x] The block and pool size must be configurable at project build time
- [x] Adapted to work on Embedded platform of different sizes in a RTOS
- [x] Unit-tests

### Description

C language library of block memory allocator with pool.

### Features:

- Interface for external implementation of memory allocation and freeing (must be thread/task safety if use in a RTOS)
- Interface for external implementation of mutex locking and unlocking for using in a RTOS
- Using the stack to manage free blocks
- Use the **BMA_BLOCK_SIZE** and **BMA_POOL_SIZE** options to set block and pool size during project building
- Internal implementation uses the **\_\_SIZE_WIDTH__** macro to get the platform bitsize
- Non pointer returned functions have an error pointer arguments to return error status:
  - if function returns with error, the error argument will be set to `1`
  - if function does not return with an error, the error argument will not be changed

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
