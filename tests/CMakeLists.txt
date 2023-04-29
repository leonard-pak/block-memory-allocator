cmake_minimum_required(VERSION 3.12)

project(block-memory-allocator-tests)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} tests.cc)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    block-memory-allocator::block-memory-allocator
    gtest_main
)

target_compile_options(${PROJECT_NAME}
  PUBLIC
    -Wall
    -Wpedantic
    -Wextra
    -Werror
    -O0
)
