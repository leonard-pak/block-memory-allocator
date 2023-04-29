cmake_minimum_required(VERSION 3.12)

project(block-memory-allocator)

set(CMAKE_CXX_STANDARD 17)

option(BMA_BUILD_TESTS "Build block memory allocator tests" OFF)
option(BMA_POOL_SIZE "Set pool size" OFF)
option(BMA_BLOCK_SIZE "Set block size" OFF)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

file(GLOB SOURCES
  src/block.cc
  src/allocator.cc
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
  target_compile_options(${PROJECT_NAME}
    PUBLIC
      -Wall
      -Wpedantic
      -Wextra
      -Werror
  )
  add_subdirectory(tests)
endif(BMA_BUILD_TESTS)
