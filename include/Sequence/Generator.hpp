#pragma once

#include "LazySequence.hpp"

template <typename T>
class LazySequence;

template <typename T>
class Generator
{
private: 
    LazySequence<T>* owner;
    size_t arity;
    std::function<T(Sequence<T>*)> rule;

public:
    Generator(LazySequence<T>* owner, size_t arity, std::function<T(Sequence<T>*)> rule) {
        this->owner = owner;
        this->rule = rule;
        this->arity = arity;
    }

    T get_next() {
        Sequence<T>* seq = new VectorSequence<T>;
        size_t size = owner->get_materialized_count();
        for (int i = 0; i < arity; i++) {
            seq->append(owner->get(size - i - 1));
        }
        T item = rule(seq);
        delete seq;
        return item;
    }

};