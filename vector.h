#ifndef STD_VECTOR_VECTOR_H_
#define STD_VECTOR_VECTOR_H_

#include <cstddef>
#include <iostream>
#include <stdexcept>

template <typename T>
class MyVector {
 public:
  MyVector() = default;

  explicit MyVector(std::size_t count)
      : data_(new T[count]()), size_(count), capacity_(count) {}

  ~MyVector() { delete[] data_; }

  MyVector(const MyVector& other)
      : data_(new T[other.capacity_]),
        size_(other.size_),
        capacity_(other.capacity_) {
    for (std::size_t i = 0; i < size_; ++i) {
      data_[i] = other.data_[i];
    }
  }

  MyVector& operator=(const MyVector& other) {
    if (this == &other) {
      return *this;
    }

    T* new_data = new T[other.capacity_];
    for (std::size_t i = 0; i < other.size_; ++i) {
      new_data[i] = other.data_[i];
    }

    delete[] data_;
    data_ = new_data;
    size_ = other.size_;
    capacity_ = other.capacity_;
    return *this;
  }

  std::size_t getSize() const { return size_; }
  std::size_t getCapacity() const { return capacity_; }

  T& operator[](std::size_t index) { return data_[index]; }
  const T& operator[](std::size_t index) const { return data_[index]; }

  void push_back(const T& value) {
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    data_[size_] = value;
    ++size_;
  }

  void pop_back() {
    if (size_ > 0) {
      --size_;
    }
  }

  void reserve(std::size_t new_capacity) {
    if (new_capacity <= capacity_) {
      return;
    }

    T* new_data = new T[new_capacity]();
    for (std::size_t i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }

    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }

  void clear() { size_ = 0; }

  T* begin() { return data_; }
  const T* begin() const { return data_; }
  // cppcheck-suppress unusedFunction
  T* end() { return data_ + size_; }
  const T* end() const { return data_ + size_; }

  bool empty() const { return size_ == 0; }

  T& at(std::size_t index) {
    CheckIndex(index);
    return data_[index];
  }

  const T& at(std::size_t index) const {
    CheckIndex(index);
    return data_[index];
  }

  T& front() {
    CheckNotEmpty();
    return data_[0];
  }

  const T& front() const {
    CheckNotEmpty();
    return data_[0];
  }

  T& back() {
    CheckNotEmpty();
    return data_[size_ - 1];
  }

  const T& back() const {
    CheckNotEmpty();
    return data_[size_ - 1];
  }

  void shrink_to_fit() {
    if (size_ == capacity_) {
      return;
    }

    T* new_data = new T[size_];
    for (std::size_t i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }

    delete[] data_;
    data_ = new_data;
    capacity_ = size_;
  }

  T* getData() { return data_; }
  const T* getData() const { return data_; }

  void resize(std::size_t new_size) {
    if (new_size > capacity_) {
      reserve(new_size);
    } else if (new_size > size_) {
      for (std::size_t i = size_; i < new_size; ++i) {
        data_[i] = T();
      }
    }
    size_ = new_size;
  }

  void insert(std::size_t index, const T& value) {
    if (index > size_) {
      throw std::out_of_range("insert index is out of range");
    }
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    for (std::size_t i = size_; i > index; --i) {
      data_[i] = data_[i - 1];
    }
    data_[index] = value;
    ++size_;
  }

  // cppcheck-suppress unusedFunction
  void print() const {
    std::cout << "Vector: ";
    for (std::size_t i = 0; i < size_; ++i) {
      std::cout << data_[i] << ' ';
    }
    std::cout << '\n';
    std::cout << "size=" << size_ << " capacity=" << capacity_ << '\n';
  }

 private:
  void CheckIndex(std::size_t index) const {
    if (index >= size_) {
      throw std::out_of_range("index is out of range");
    }
  }

  void CheckNotEmpty() const {
    if (empty()) {
      throw std::out_of_range("vector is empty");
    }
  }

  T* data_ = nullptr;
  std::size_t size_ = 0;
  std::size_t capacity_ = 0;
};

#endif  // STD_VECTOR_VECTOR_H_
