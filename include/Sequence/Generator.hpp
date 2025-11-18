#pragma once

#include "LazySequence.hpp"

template <typename T>
class LazySequence;

template <typename T>
class Generator
{
public:
    virtual T get_next() = 0;
    virtual Generator<T>* clone(LazySequence<T>* new_owner) = 0; 
};

template <typename T>
class Index_Generator : public Generator<T> {
private:
    LazySequence<T>* owner;
    std::function<T(int)> rule;
    int current_index;

public:
    Index_Generator(LazySequence<T>* owner, int start_index, std::function<T(int)> rule) {
        this->current_index = start_index;
        this->owner = owner;
        this->rule = rule;
    }

    Generator<T>* clone(LazySequence<T>* new_owner) {
        return new Index_Generator<T>(new_owner, current_index, rule);
    }

    T get_next() override {
        T item = rule(current_index);
        current_index++;
        return item;
    }
};

template <typename T>
class Sequence_Generator : public Generator<T>
{
private: 
    LazySequence<T>* owner;
    VectorSequence<T> rule_args;
    std::function<T(Sequence<T>*)> rule;

public:
    Sequence_Generator(LazySequence<T>* owner, size_t arity, std::function<T(Sequence<T>*)> rule) {
        this->rule_args = VectorSequence<T>(arity);
        this->owner = owner;
        this->rule = rule;
    }

    Generator<T>* clone(LazySequence<T>* new_owner) {
        return new Sequence_Generator<T>(new_owner, rule_args.get_size(), rule);
    }

    T get_next() override {
        size_t size = owner->get_materialized_count();
        for (size_t i = 0; i < rule_args.get_size(); i++) {
            rule_args.set(i, owner->get(size - 1 - i));
        }
        T item = rule(&rule_args);
        return item;
    }

};