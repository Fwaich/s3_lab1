#pragma once
#include <sstream>
#include <stdexcept>
#include <string>
#include "Sequence.hpp"
#include "DynamicArray.hpp"

template <typename T>
class Array_Sequence : public Sequence<T>
{
private:
    Dynamic_Array<T> array;

public:
    Array_Sequence() = default;

    Array_Sequence(int initial_size) : array(initial_size) {} 

    Array_Sequence(const T* arr, int count) : array(arr, count) {}

    Array_Sequence(const Array_Sequence<T>& other) : array(other.array) {}

    Array_Sequence(const Sequence<T>* seq) : array(seq->get_size()) {
        for (int i = 0; i < seq->get_size(); i++) {
            array.set(i, seq->get(i));
        }
    }

    ~Array_Sequence() override = default;

    Array_Sequence<T>* append(const T& item) override {
        array.push_back(item);
        return this;
    }

    Array_Sequence<T>* prepend(const T& item) override {
        array.push_front(item);
        return this;
    }

    Array_Sequence<T>* set(int index, const T& item) override {
        array.set(index, item);
        return this;
    }

    Array_Sequence<T>* insert_at(int index, const Sequence<T>* other_seq) override {
        if (index < 0 || index > array.get_size()) {
            throw std::out_of_range("Index out of range");
        }
        
        if (other_seq == nullptr) {
            throw std::invalid_argument("Other sequence cannot be null");
        }

        int other_size = other_seq->get_size();

        if (index == array.get_size()) {
            for (int i = 0; i < other_size; i++) {
                array.push_back(other_seq->get(i));
            }
            return this;
        }

        if (index == 0) {

            for (int i = other_size - 1; i >= 0; --i) {
                array.push_front(other_seq->get(i));
            }
            return this;
        }

        Dynamic_Array<T> temp;
        
        for (int i = 0; i < index; i++) {
            temp.push_back(array.get(i));
        }
        
        for (int i = 0; i < other_size; i++) {
            temp.push_back(other_seq->get(i));
        }
        
        for (int i = index; i < array.get_size(); i++) {
            temp.push_back(array.get(i));
        }
        
        array = std::move(temp);
        return this;
    }

    T get(int index) const override {
        return array.get(index);
    }

    T get_first() const override {
        if (array.get_size() == 0)
            throw std::runtime_error("Sequence is empty");
        return array.get(0);
    }

    T get_last() const override {
        int vector_size = array.get_size();
        if (vector_size == 0)
            throw std::runtime_error("Sequence is empty");
        return array.get(vector_size - 1);
    }

    int get_size() const override {
        return array.get_size();
    }

    Array_Sequence<T>* get_subsequence(int start_index, int end_index) const override {
        if (array.get_size() == 0)
            throw std::runtime_error("Sequence is empty");

        if (start_index < 0 || end_index >= array.get_size() || start_index > end_index)
            throw std::out_of_range("Invalid subsequence range");

        int sub_size = end_index - start_index + 1;
        Array_Sequence<T>* sub = new Array_Sequence<T>;

        for (int i = start_index; i < end_index + 1; i++) {
            T item = array.get(i);
            sub->append(item);
        }

        return sub;
    }

    Array_Sequence<T>* map(std::function<T(T)> func ) override {
        Array_Sequence<T>* mapped_vector = new Array_Sequence<T>(array.get_size());

        for (int i = 0; i < array.get_size(); i++){
            T mapped_item = func(array.get(i));
            mapped_vector->set(i, mapped_item);
        }

        return mapped_vector;
    }

    Array_Sequence<T>* reset() override {
        array.reset();

        return this;
    }

    //отдельный модуль за вывод
    std::string to_string() const override {
        std::ostringstream oss;
        oss << "[";
        for (int i = 0; i < array.get_size(); i++){
            oss << array.get(i);
            if (i + 1 < array.get_size())  oss << ", ";
        }
        oss << "]";
        return oss.str();
    }
};
