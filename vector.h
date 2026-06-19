#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>    
#include <initializer_list> 
#include <iterator>     
#include <memory>       
#include <stdexcept>   


//шаблонный класс динамического массива (аналог std::vector)
template <typename T>
class Myvector{
    public:
    //типы-псевдонимы для совместимости с STL
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;
    using Pointer = T*;
    using const_pointer = const T*;
    using difference_type = std::ptrdiff_t;

    private:

    pointer data_ = nullptr;    
    size_type size_ = 0;        
    size_type capacity_ = 0;   

    
    //деаллокация памяти
    void deallocate()noexcept{
        if(data_){
            std::allocate<T> alloc;
            alloc.deallocate(data_, capacity_);
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
        }
    }

    //удаление объектов в диапазоне 
    void destroy_range(pointer first, pointer last) noexcept {
        for (pointer p = first; p != last; ++p) {
            p->~T();
        }
    }
    
    //аллокация памяти под new_capacity элементов
    void allocate(size_type new_capacity) {
        if(new_capacity == 0) return;

        std::allocator<T> alloc;
        data_ = alloc.allocate(new_capacity);
        capacity_ = new_capacity;
    }

    void reallocate(size_type new_capacity) {
        if (new_capacity == 0) {
            deallocate();
            return;
        }
        
        if (capacity_ == new_capacity) return;
        
        std::allocator<T> alloc;
        pointer new_data = alloc.allocate(new_capacity);
        
        size_type old_size = size_;
        
        if (data_) {
            for (size_type i = 0; i < size_; ++i) {
                new (new_data + i) T(std::move(data_[i]));
                data_[i].~T();
            }
            deallocate();
        }
        
        data_ = new_data;
        capacity_ = new_capacity;
        size_ = old_size;
    }

    //функция для копирования диапазона элементов
    void construct_from_range(const_iterator first, const_iterator last) {
        size_type count = 0;
        for (auto it = first; it != last; ++it) {
            ++count;
        }
        
        if (count == 0) return;
        
        allocate(count);
        size_ = count;
        
        size_type i = 0;
        for (auto it = first; it != last; ++it) {
            new (data_ + i) T(*it);
            ++i;
        }
    }


public:

 MyVector() noexcept = default;

    explicit MyVector(size_type count) {
        if (count > 0) {
            allocate(count);
            size_ = count;
            for (size_type i = 0; i < size_; ++i) {
                new (data_ + i) T();
            }
        }
    }

    //конструктор с заданным количеством элементов и значением
    MyVector(size_type count, const T& value) {
        if (count > 0) {
            allocate(count);
            size_ = count;
            for (size_type i = 0; i < size_; ++i) {
                new (data_ + i) T(value);
            }
        }
    }

    //конструктор для списка списка
    MyVector(std::initializer_list<T> init) {
        construct_from_range(init.begin(), init.end());
    }

    //конструктор копирования
    MyVector(const MyVector& other) {
        construct_from_range(other.begin(), other.end());
    }

    ~MyVector() {
        destroy_range(data_, data_ + size_);
        deallocate();
    }

    //оператор присваивания
    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            MyVector temp(other);
            swap(temp);
        }
        return *this;
    }

};
#endif // VECTOR_H