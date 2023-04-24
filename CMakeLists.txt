cmake_minimum_required(VERSION 3.0.0)

project(block-memory-allocator)

option(BLOCK_MEMORY_ALLOC_BUILD_TESTS "Build block memory allocator tests" OFF)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

file(GLOB SOURCES src/block-memory-allocator.c)

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC inc
)

if(BLOCK_MEMORY_ALLOC_BUILD_TESTS)
  add_subdirectory(tests)
endif(BLOCK_MEMORY_ALLOC_BUILD_TESTS)
