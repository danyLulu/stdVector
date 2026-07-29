#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>

#define MESSAGE_FROM_GEORGE  // can be deleted

#ifdef MESSAGE_FROM_GEORGE
class TemplateClass {
   public:
    TemplateClass() { std::cout << "CONSTRUCTOR OF TEMPLATE_CLASS" << std::endl; }

    ~TemplateClass() { std::cout << "DESTRUCTOR OF TEMPLATE_CLASS" << std::endl; }
};
#endif

template <typename T>
class MyVector {
   private:
    T *data;
    size_t size;
    size_t capacity;

   public:
    MyVector() : data(nullptr), size(0), capacity(0) {}

    explicit MyVector(size_t count) : size(count), capacity(count) { data = new T[count]; }

    ~MyVector() {
#ifndef MESSAGE_FROM_GEORGE
        delete[] data;
#endif

#ifdef MESSAGE_FROM_GEORGE
        // Вызываем деструкторы только для созданных элементов (от 0 до size)
        for (size_t i = 0; i < size; ++i) {
            data[i].~T();
        }

        // Освобождаем "сырую" память массива
        // (в зависимости от того, как вы её выделяли — например, через operator
        // delete)
        ::operator delete(data);
#endif
    }

    // получение размера
    size_t getSize() const { return size; }

    // получение вместимости
    size_t getCapacity() const { return capacity; }

    T &operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("индекс выходит за пределы диапазона");  // Поменять на инглиш
        }
        return data[index];
    }

    const T &operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("индекс выходит за пределы диапазона");  // Поменять на инглиш
        }
        return data[index];
    }

    void push_back(const T &value) noexcept {
        if (size >= capacity) {
            size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
            reserve(new_capacity);
        }
#ifndef MESSAGE_FROM_GEORGE
        data[size++] = value;
#endif

#ifdef MESSAGE_FROM_GEORGE
        new (&data[size]) T(value);  // Конструирование объект прямо в выделенной памяти
        size++;
#endif
    }

    void pop_back() {
        if (size > 0) {
            size--;
            data[size].~T();
        }
    }

#ifndef MESSAGE_FROM_GEORGE
    void reserve(size_t new_capacity) noexcept {
        if (new_capacity > capacity) {
            T *new_data = new T[new_capacity];
            for (size_t i = 0; i < size; i++) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }
    }
#endif

#ifdef MESSAGE_FROM_GEORGE
    void reserve(size_t new_capacity) {  // без noexcept, память может не выделиться
        if (new_capacity > capacity) {
            // Выделение памяти БЕЗ!!! вызова конструкторов
            T *new_data = static_cast<T *>(::operator new(new_capacity * sizeof(T)));

            // 2. Переносим (или копируем) существующие элементы в новую память
            for (size_t i = 0; i < size; ++i) {
                // Используем специальный new для конструирования объекта в конкретном месте
                new (&new_data[i]) T(data[i]);
            }

            for (size_t i = 0; i < size; ++i) {
                data[i].~T();
            }

            ::operator delete(data);

            data = new_data;
            capacity = new_capacity;
        }
    }
#endif

    void clear() { size = 0; }

    // Итераторы
    T *begin() { return data; }
    T *end() { return data + size; }
    const T *begin() const { return data; }
    const T *end() const { return data + size; }

    // проверка на пустоту
    bool empty() const { return size == 0; }

    // доступ с проверкой границ
    T &at(size_t index) {
        if (index >= size) {
            throw std::out_of_range("индекс выходит за пределы диапазона");
        }
        return data[index];
    }

    const T &at(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("индекс выходит за пределы диапазона");
        }
        return data[index];
    }

    // первый и последний элемент
    T &front() { return data[0]; }

    const T &front() const { return data[0]; }

    T &back() { return data[size - 1]; }

    const T &back() const { return data[size - 1]; }

    // урезать capacity до size
    void shrink_to_fit() {
        if (size < capacity) {
            reserve(size);
        }
    }

    // сырой указатель на данные
    // cppcheck-suppress unusedFunction
    T *data_ptr() { return data; }
    const T *data_ptr() const { return data; }

#ifndef MESSAGE_FROM_GEORGE
    // изменить размер
    void resize(size_t new_size) {
        if (new_size > size) {
            if (new_size > capacity) reserve(new_size);
            for (size_t i = size; i < new_size; ++i) data[i] = T();
        }
        size = new_size;
    }
#endif

#ifdef MESSAGE_FROM_GEORGE
    void resize(size_t new_size) {
        if (new_size > size) {
            // Расширение
            if (new_size > capacity) {
                reserve(new_size);
            }
            for (size_t i = size; i < new_size; ++i) {
                new (&data[i]) T();
            }
        } else if (new_size < size) {
            // Сужение
            for (size_t i = new_size; i < size; ++i) {
                data[i].~T();
            }
        }

        size = new_size;
    }
#endif

    void insert(size_t index, const T &value) {
        if (index >= size) {
            throw std::out_of_range("индекс выходит за пределы диапазона");
        }
        if (size >= capacity) {
            size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
            reserve(new_capacity);
        }
        for (size_t i = size; i > index; --i) data[i] = data[i - 1];
        data[index] = value;
        ++size;
    }

    // конструктор копирования
    MyVector(const MyVector &other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
    }

    // оператор присваивания
    MyVector &operator=(const MyVector &other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
        }
        return *this;
    }
};
