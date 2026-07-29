cmake_minimum_required(VERSION 3.14)

project(std_vector_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
find_package(GTest REQUIRED)

add_executable(vector_test tests/vector_test.cpp)
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
