#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>

#include "vector.h"

namespace {

using Clock = std::chrono::steady_clock;
constexpr std::size_t kElementCount = 1'000'000;
constexpr std::size_t kInsertCount = 10'000;

volatile std::uint64_t checksum = 0;

template <typename Container>
void Consume(const Container& container) {
  const std::uint64_t result = std::accumulate(
      container.begin(), container.end(), std::uint64_t{0},
      [](std::uint64_t total, const auto& value) {
        return total + static_cast<std::uint64_t>(value);
      });
  checksum += result;
}

template <typename Container>
double PushBack(bool reserve_first) {
  Container container;
  if (reserve_first) {
    container.reserve(kElementCount);
  }

  const auto start = Clock::now();
  for (std::size_t i = 0; i < kElementCount; ++i) {
    container.push_back(static_cast<int>(i));
  }
  const auto elapsed = Clock::now() - start;
  Consume(container);
  return std::chrono::duration<double, std::milli>(elapsed).count();
}

template <typename Container>
double RandomAccess() {
  Container container;
  container.reserve(kElementCount);
  for (std::size_t i = 0; i < kElementCount; ++i) {
    container.push_back(static_cast<int>(i));
  }

  std::uint64_t result = 0;
  const auto start = Clock::now();
  for (std::size_t i = 0; i < kElementCount; ++i) {
    result += static_cast<std::uint64_t>(container[(i * 7919) % kElementCount]);
  }
  const auto elapsed = Clock::now() - start;
  checksum += result;
  return std::chrono::duration<double, std::milli>(elapsed).count();
}

template <typename Container>
double InsertInMiddle() {
  Container container;
  container.reserve(kInsertCount * 2);
  for (std::size_t i = 0; i < kInsertCount; ++i) {
    container.push_back(static_cast<int>(i));
  }

  const auto start = Clock::now();
  for (std::size_t i = 0; i < kInsertCount; ++i) {
    if constexpr (std::is_same_v<Container, MyVector<int>>) {
      container.insert(container.getSize() / 2, static_cast<int>(i));
    } else {
      const std::size_t middle = container.size() / 2;
      container.insert(container.begin() + static_cast<std::ptrdiff_t>(middle),
                       static_cast<int>(i));
    }
  }
  const auto elapsed = Clock::now() - start;
  Consume(container);
  return std::chrono::duration<double, std::milli>(elapsed).count();
}

void PrintResult(const std::string& operation, double my_vector_time,
                 double std_vector_time) {
  std::cout << std::left << std::setw(28) << operation << std::right
            << " MyVector: " << std::setw(10) << std::fixed
            << std::setprecision(3) << my_vector_time << " ms"
            << " | std::vector: " << std::setw(10) << std_vector_time
            << " ms\n";
}

}  // namespace

int main() {
  try {
    std::cout << "Elements: " << kElementCount << ", inserts: "
              << kInsertCount << "\n\n";

    PrintResult("push_back (growth)", PushBack<MyVector<int>>(false),
                PushBack<std::vector<int>>(false));
    PrintResult("push_back (reserved)", PushBack<MyVector<int>>(true),
                PushBack<std::vector<int>>(true));
    PrintResult("operator[] access", RandomAccess<MyVector<int>>(),
                RandomAccess<std::vector<int>>());
    PrintResult("insert (middle)", InsertInMiddle<MyVector<int>>(),
                InsertInMiddle<std::vector<int>>());

    std::cout << "checksum: " << checksum << '\n';
  } catch (const std::exception& error) {
    std::cerr << "Benchmark failed: " << error.what() << '\n';
    return 1;
  }
}
