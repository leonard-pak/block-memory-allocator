cmake_minimum_required(VERSION 3.0.0)

project(block-memory-allocator-tests)

add_executable(${PROJECT_NAME} tests.cc)

target_link_libraries(${PROJECT_NAME} PRIVATE block-memory-allocator::block-memory-allocator)
