#pragma once
#include <sstream>
#include <stdexcept>
#include <string>
#include "Sequence.hpp"
#include "Vector.hpp"

template <typename T>
class Vector_Sequence : public Sequence<T>
{
private:
    Vector<T> vector;

public:
    Vector_Sequence() = default;

    Vector_Sequence(int initial_size) : vector(initial_size) {} 

    Vector_Sequence(const T* arr, int count) : vector(arr, count) {}

    Vector_Sequence(const Vector_Sequence<T>& other) : vector(other.vector) {}

    Vector_Sequence(const Sequence<T>* seq) : vector(seq->get_size()) {
        for (int i = 0; i < seq->get_size(); ++i) {
            vector.set(i, seq->get(i));
        }
    }

    ~Vector_Sequence() override = default;

    Vector_Sequence<T>* append(const T& item) override {
        vector.push_back(item);
        return this;
    }

    Vector_Sequence<T>* prepend(const T& item) override {
        vector.push_front(item);
        return this;
    }

    Vector_Sequence<T>* set(int index, const T& item) override {
        vector.set(index, item);
        return this;
    }

    T get(int index) const override {
        return vector.get(index);
    }

    T get_first() const override {
        if (vector.get_size() == 0)
            throw std::runtime_error("Sequence is empty");
        return vector.get(0);
    }

    T get_last() const override {
        int vector_size = vector.get_size();
        if (vector_size == 0)
            throw std::runtime_error("Sequence is empty");
        return vector.get(vector_size - 1);
    }

    int get_size() const override {
        return vector.get_size();
    }

    Vector_Sequence<T>* get_subsequence(int start_index, int end_index) const override {
        if (vector.get_size() == 0)
            throw std::runtime_error("Sequence is empty");

        if (start_index < 0 || end_index >= vector.get_size() || start_index > end_index)
            throw std::out_of_range("Invalid subsequence range");

        int sub_size = end_index - start_index + 1;
        Vector_Sequence<T>* sub = new Vector_Sequence<T>;

        for (int i = start_index; i < end_index + 1; ++i) {
            T item = vector.get(i);
            sub->append(item);
        }

        return sub;
    }

    Vector_Sequence<T>* map(std::function<T(T)> func ) override {
        Vector_Sequence<T>* mapped_vector = new Vector_Sequence<T>(vector.get_size());

        for (int i = 0; i < vector.get_size(); ++i){
            T mapped_item = func(vector.get(i));
            mapped_vector->set(i, mapped_item);
        }

        return mapped_vector;
    }

    Vector_Sequence<T>* reset() override {
        vector.reset();

        return this;
    }

    std::string to_string() const override {
        std::ostringstream oss;
        oss << "[";
        for (int i = 0; i < vector.get_size(); ++i){
            oss << vector.get(i);
            if (i + 1 < vector.get_size())  oss << ", ";
        }
        oss << "]";
        return oss.str();
    }
};
