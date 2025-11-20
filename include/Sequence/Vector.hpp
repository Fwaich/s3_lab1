#pragma once
#include <stdexcept>

template <typename T>
class Vector {
private:
    T* data;
    int size;
    int capacity;

    void ensure_capacity(int min_capacity) {
        if (min_capacity <= capacity)
            return;

        int new_capacity = (capacity == 0) ? 1 : capacity * 2;

        T* new_data = new T[new_capacity];

        for (int i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }

        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    Vector() : data(nullptr), size(0), capacity(0) {}

    Vector(int initial_size) : size(initial_size), capacity(initial_size) {
        data = new T[capacity];
    }

    Vector(const T* arr, int count) : size(count), capacity(count) {
        data = new T[capacity];
        for (int i = 0; i < count; ++i) {
            data[i] = arr[i];
        }
    }

    Vector(const Vector<T>& other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    ~Vector() {
        if (data) delete[] data;
    }

    void push_back(const T& value) {
        ensure_capacity(size + 1);
        data[size++] = value;
    }

    void push_front(const T& value) {
        ensure_capacity(size + 1);
        for (int i = size; i > 0; --i) {
            data[i] = data[i - 1];
        }

        data[0] = value;
        ++size;
    }

    void set(int index, const T& value) {
        if (index < 0 || index >= size)
            throw std::out_of_range("Vector::set index out of range");
        data[index] = value;
    }

    T get(int index) const {
        if (index < 0 || index >= size)
            throw std::out_of_range("Vector::get index out of range");
        return data[index];
    }

    int get_size() const {
        return size; 
    }

    void reset() {
        if (!data) return;
        delete[] data;
        data = nullptr;
        size = 0;
        capacity = 0;
    }
};
