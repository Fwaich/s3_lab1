#pragma once
#include <iostream>
#include "exceptions.h"

using namespace std;

template <typename T>
class DynamicArray 
{
private:    
    T* data;
    int size;
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
DynamicArray<T>::DynamicArray(): data(nullptr), size(0) {};

template <typename T>
DynamicArray<T>::DynamicArray(int size_value){
    size = size_value;
    data = new T[size];
};

template <typename T>
DynamicArray<T>::DynamicArray(T* items, int count){
    size = count;
    data = new T[size];
    for (int i = 0; i < size; i++){
        data[i] = items[i];
    }
};

template <typename T>
DynamicArray<T>::~DynamicArray(){
    delete[] data;
};

template <typename T>
T DynamicArray<T>::get(int index) const {
    if (index >= size || index < 0) throw array_out_of_range();

    return data[index];

}

template <typename T>
int DynamicArray<T>::get_size() const {
    return size;
};

template <typename T>
void DynamicArray<T>::set(int index, T value){
    if (index >= size || index < 0)  throw array_out_of_range();

    data[index] = value; 

};

template <typename T>
void DynamicArray<T>::resize(int new_size){
    if (new_size < 0){
        throw invalid_argument("Array: New size must be non-negative");
        return;
    }
    
    T* tmp_data = new T[new_size];
    int size_to_copy = (new_size > size) ? size : new_size;
    for (int i = 0; i < new_size; i++){
        tmp_data[i] = (i < size_to_copy) ? data[i] : T();
    } 

    delete[] data;
    data = tmp_data;
    size = new_size; 
};

template <typename T>
void DynamicArray<T>::print(){
    if (size == 0){
        cout << "Empty" << endl;
        return;
    }

    for (int i = 0; i < size; i++){
        cout << "[" << i << "]: " << data[i] << endl;
    }
}