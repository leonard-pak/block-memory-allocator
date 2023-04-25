cmake_minimum_required(VERSION 3.12)

project(block-memory-allocator)

option(BMA_BUILD_TESTS "Build block memory allocator tests" OFF)
option(BMA_POOL_SIZE "Build block memory allocator tests" OFF)
option(BMA_BLOCK_SIZE "Build block memory allocator tests" OFF)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

file(GLOB SOURCES
  src/block-memory-allocator.c
  src/simple-stack.c
)

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC inc
)

if(BMA_POOL_SIZE)
  target_compile_definitions(${PROJECT_NAME}
    PUBLIC BMA_POOL_SIZE=${BMA_POOL_SIZE}
  )
endif(BMA_POOL_SIZE)

if(BMA_BLOCK_SIZE)
  target_compile_definitions(${PROJECT_NAME}
    PUBLIC BMA_BLOCK_SIZE=${BMA_BLOCK_SIZE}
  )
endif(BMA_BLOCK_SIZE)

if(BMA_BUILD_TESTS)
  add_subdirectory(tests)
endif(BMA_BUILD_TESTS)
