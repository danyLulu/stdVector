cmake_minimum_required(VERSION 3.14)

project(std_vector_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()
find_package(GTest QUIET)

if(GTest_FOUND)
  add_executable(vector_test tests/vector_test.cpp)
  target_include_directories(vector_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(vector_test PRIVATE GTest::gtest_main)
  target_compile_options(vector_test PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
  )

  include(GoogleTest)
  gtest_discover_tests(vector_test)
else()
  message(STATUS "GoogleTest not found; skipping vector_test")
endif()

add_executable(vector_benchmark benchmarks/vector_benchmark.cpp)
target_include_directories(vector_benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(vector_benchmark PRIVATE
  -O2
  -Wall
  -Wextra
  -Wpedantic
  -Wshadow
  -Wconversion
  -Wsign-conversion
)
