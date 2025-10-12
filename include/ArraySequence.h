#pragma once
#include <type_traits>
#include <sstream>
#include <iomanip>
#include "DynamicArray.h"
#include "Sequence.h"
#include "exceptions.h"

template <typename T>
class ArraySequence : public Sequence<T>
{
private:
    DynamicArray<T>* items;
public:

    ArraySequence();
    ArraySequence(T* arr, int count);
    ArraySequence( Sequence<T>& seq);
    ~ArraySequence() override;

    ArraySequence<T>* append(T item) override;
    ArraySequence<T>* prepend(T item) override;
    ArraySequence<T>* set(int index, T item) override;

    T get(int index) const override;
    T get_first() const override;
    T get_last() const override;
    int get_size() const override;

    ArraySequence<T>* get_subsequence(int start_index, int end_index) const override;

    ArraySequence<T>* map(double multiplier) override;

    std::string to_string() const override;

};

template <typename T>
ArraySequence<T>::ArraySequence(){
    items = new DynamicArray<T>;
}

template <typename T>
ArraySequence<T>::ArraySequence(T* arr, int count){
    items = new DynamicArray<T>(arr, count);
}

template <typename T>
ArraySequence<T>::ArraySequence( Sequence<T>& seq){
    items = new DynamicArray<T>(seq.get_size());

    for (int i = 0; i < items->get_size(); i++){
        set(i, seq.get(i));
    }
}

template <typename T>
ArraySequence<T>::~ArraySequence(){
    delete items;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::append(T item){
    int new_size = items->get_size() + 1;
    items->resize(new_size);

    items->set(new_size - 1, item);

    return this;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::prepend(T item){
    int new_size = items->get_size() + 1;
    items->resize(new_size);

    for (int i = new_size - 1; i > 0; i--) {
        items->set(i, items->get(i - 1));
    }

    items->set(0, item);

    return this;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::set(int index, T item){
    items->set(index, item);

    return this;
}

template <typename T>
T ArraySequence<T>::get(int index) const {
    return items->get(index);
}

template <typename T>
int ArraySequence<T>::get_size() const {
    return items->get_size();
}

template <typename T>
T ArraySequence<T>::get_first() const {
    if (items->get_size() == 0) throw array_out_of_range();
    return items->get(0);
}

template <typename T>
T ArraySequence<T>::get_last() const {
    int size_of_array = items->get_size();
    if (size_of_array == 0) throw array_out_of_range();
    return items->get(size_of_array - 1);
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::get_subsequence(int start_index, int end_index) const {
    if (!items) throw data_is_null();
    if (get_size() == 0) throw data_is_null();

    if (start_index < 0 || start_index >= items->get_size() ||
        end_index < 0 || end_index >= items->get_size()) {
        throw array_out_of_range();
    }

    if (start_index > end_index){
        int tmp = end_index;
        end_index = start_index;
        start_index = tmp;
    }

    int sub_size = end_index - start_index + 1;
    T* sub_data = new T[sub_size];

    int j = 0;
    for (int i = start_index; i < end_index + 1; i++) {
        sub_data[j++] = items->get(i);
    }

    ArraySequence<T>* sub_array = new ArraySequence<T>(sub_data, sub_size);
    delete[] sub_data;
    return sub_array;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::map(double multiplier) {
    if (get_size() == 0) throw data_is_null();

    for (int i = 0; i < items->get_size(); ++i) {
        T value = items->get(i);
        if constexpr (std::is_same_v<T, std::string>) {
            for (char& c : value) {
                c = std::toupper(static_cast<unsigned char>(c));
            }
            items->set(i, value);
        } else {
            items->set(i, value * multiplier);
        }
    }
    return this;
}


template <typename T>
std::string ArraySequence<T>::to_string() const {
    std::string result = "[";

    for (int i = 0; i < items->get_size(); ++i) {
        if constexpr (std::is_same_v<T, std::string>) {
            result += items->get(i);
        } else if constexpr (std::is_same_v<T, double>) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(3) << items->get(i);
            result += oss.str();
        } else {
            result += std::to_string(items->get(i));
        }

        if (i != items->get_size() - 1) {
            result += ", ";
        }
    }

    result += "]";
    return result;
}

