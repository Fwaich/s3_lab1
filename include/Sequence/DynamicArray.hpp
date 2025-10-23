#pragma once
#include <iostream>
#include "exceptions.hpp"

template <typename T>
class DynamicArray 
{
private:    
    T* data;
    int size;
    int capacity;

private:
    void reallocate(int new_capacity);

public:
    DynamicArray();
    DynamicArray(int size_value);
    DynamicArray(T* items, int count);
    ~DynamicArray();

    T get(int index) const;
    int get_size() const;
    void set(int index, T value);
    void resize(int new_size);

    void print();
};

template <typename T>
DynamicArray<T>::DynamicArray(): data(nullptr), size(0), capacity(0) {};

template <typename T>
DynamicArray<T>::DynamicArray(int size_value) {
    size = size_value;
    capacity = size;
    data = new T[capacity];
    
    for (int i = 0; i < size; i++) {
        data[i] = T();
    }
};

template <typename T>
DynamicArray<T>::DynamicArray(T* items, int count) {
    size = count;
    capacity = size;
    data = new T[capacity];
    for (int i = 0; i < size; i++) {
        data[i] = items[i];
    }
};

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
};

template <typename T>
void DynamicArray<T>::reallocate(int new_capacity) {
    T* tmp_data = new T[new_capacity];
    
    int elements_to_copy = (size < new_capacity) ? size : new_capacity;
    for (int i = 0; i < elements_to_copy; i++) {
        tmp_data[i] = data[i];
    }
    
    for (int i = elements_to_copy; i < new_capacity; i++) {
        tmp_data[i] = T();
    }

    delete[] data;
    data = tmp_data;
    capacity = new_capacity;
    
    if (new_capacity < size) {
        size = new_capacity;
    }
}

template <typename T>
T DynamicArray<T>::get(int index) const {
    if (index >= size || index < 0) throw out_of_range();
    return data[index];
}

template <typename T>
int DynamicArray<T>::get_size() const {
    return size;
};

template <typename T>
void DynamicArray<T>::set(int index, T value) {
    if (index >= size || index < 0) throw out_of_range();
    data[index] = value; 
};

template <typename T>
void DynamicArray<T>::resize(int new_size) {
    if (new_size < 0) {
        throw std::invalid_argument("Array: New size must be non-negative");
    }
    
    if (new_size > capacity) {
        int new_capacity;
        if (capacity == 0) {
            new_capacity = (new_size > 1) ? new_size : 1; 
        } else {
            new_capacity = (capacity * 2 > new_size) ? capacity * 2 : new_size;
        }
        reallocate(new_capacity);
    }
    
    if (new_size > size) {
        for (int i = size; i < new_size; i++) {
            data[i] = T();
        }
    }
    
    size = new_size;
};

template <typename T>
void DynamicArray<T>::print() {
    if (size == 0) {
        std::cout << "Empty" << std::endl;
        return;
    }

    for (int i = 0; i < size; i++) {
        std::cout << "[" << i << "]: " << data[i] << std::endl;
    }
}