#include <cstddef>
#include <exception>
#include <iostream>

#include "vector.h"

int main() {
  try {
    MyVector<int> vector;

  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);

  std::cout << "Элементы вектора\n";
  for (std::size_t i = 0; i < vector.getSize(); ++i) {
    std::cout << vector[i] << '\n';
  }

  std::cout << "Размер вектора: " << vector.getSize() << '\n';
  std::cout << "Емкость вектора: " << vector.getCapacity() << '\n';

  vector.pop_back();
  std::cout << "Размер после pop_back: " << vector.getSize() << '\n';
  std::cout << "empty: " << vector.empty() << '\n';
  std::cout << "front: " << vector.front() << '\n';
  std::cout << "back: " << vector.back() << '\n';
  std::cout << "at(1): " << vector.at(1) << '\n';

  std::cout << "range-based for: ";
  for (int value : vector) {
    std::cout << value << ' ';
  }
  std::cout << '\n';

  vector.resize(5);
  std::cout << "После resize(5): ";
  for (int value : vector) {
    std::cout << value << ' ';
  }
  std::cout << '\n';

  vector.shrink_to_fit();
  std::cout << "Capacity после shrink_to_fit: " << vector.getCapacity()
            << '\n';

  vector.insert(1, 4);
  static_cast<void>(vector.end());
  vector.print();
  std::cout << "После insert(1, 4): ";
  for (int value : vector) {
    std::cout << value << ' ';
  }
  std::cout << '\n';

    const MyVector<int>& copy = vector;
    std::cout << "copy (копия): ";
    for (int value : copy) {
      std::cout << value << ' ';
    }
    std::cout << '\n';

    MyVector<int> assigned;
    assigned = vector;
    std::cout << "assigned (присваивание): ";
    for (int value : assigned) {
      std::cout << value << ' ';
    }
    std::cout << '\n';
  } catch (const std::exception& error) {
    std::cerr << "Program failed: " << error.what() << '\n';
    return 1;
  }
}
