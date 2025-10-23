#pragma once
#include <string>
#include <functional>

template <typename T>
class Sequence
{
public:

    virtual ~Sequence() = 0;

    virtual Sequence<T>* append(T item) = 0;
    virtual Sequence<T>* prepend(T item) = 0;
    virtual Sequence<T>* set(int index, T item) = 0;
    virtual T get(int index) const = 0;
    virtual T get_first() const = 0;
    virtual T get_last() const = 0;
    virtual int get_size() const = 0;
    virtual Sequence<T>* get_subsequence(int start_index, int end_index) const = 0;
    virtual Sequence<T>* map(std::function<T(T)> func) = 0;

    virtual std::string to_string() const = 0;
};

template <typename T>
Sequence<T>::~Sequence() {}