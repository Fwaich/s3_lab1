#pragma once

#include "LazySequence.hpp"
#include <memory>

template <typename T>
class Lazy_Sequence;

template <typename T>
class Generator
{
public:
    virtual T get_next() = 0;
    virtual bool has_next() = 0;
    // virtual Generator<T>* clone(Lazy_Sequence<T>* new_owner) = 0; 
    virtual ~Generator() = default;
};

template <typename T>
class Function_Generator : public Generator<T>
{
private: 
    Lazy_Sequence<T>* owner;
    Vector_Sequence<T> args_buffer;
    std::function<T(Sequence<T>*)> rule;
    bool is_infinite;

public:
    Function_Generator(Lazy_Sequence<T>* owner, size_t arity, std::function<T(Sequence<T>*)> rule) {
        this->args_buffer = Vector_Sequence<T>(arity);
        this->owner = owner;
        this->rule = rule;
        this->is_infinite = true;
    }
    
    ~Function_Generator() {}

    // Generator<T>* clone(Lazy_Sequence<T>* new_owner) {
    //     return new Function_Generator<T>(new_owner, args_buffer.get_size(), rule);
    // }

    T get_next() override {
        if (owner->get_materialized_count() < args_buffer.get_size())
            throw std::runtime_error("Not enough elements to generate next");

        size_t size = owner->get_materialized_count();
        for (size_t i = 0; i < args_buffer.get_size(); i++) {
            args_buffer.set(i, owner->get(size - 1 - i));
        }
        T item = rule(&args_buffer);
        return item;
    }

    bool has_next() {
        return this->is_infinite; 
    }

};

template <typename T>
class Sequence_Generator : public Generator<T> 
{
private:
    Sequence<T>* seq;
    size_t current_index;

public:
    Sequence_Generator(Sequence<T>* seq){ 
        this->seq = seq;
        this->current_index = 0;
    } 

    Sequence_Generator(Sequence<T>* seq, size_t current_index){ 
        this->seq = seq;
        this->current_index = current_index;
    } 

    ~Sequence_Generator() {
        delete seq;
    }

    // Generator<T>* clone(Lazy_Sequence<T>* new_owner) override {
    //     Sequence<T>* seq_copy = seq->get_subsequence(0, seq->get_size() - 1);
    //     return new Sequence_Generator(seq_copy);
    // }

    T get_next() override {
        return seq->get(current_index++);
    }

    bool has_next() override {
        return current_index < seq->get_size();
    }
};

template <typename T>
class Concat_Generator : public Generator<T> 
{
private:
    Generator<T>* primary;
    Generator<T>* secondary;

public:
    Concat_Generator(Generator<T>* primary_generator, Generator<T>* secondary_generator) {
        this->primary = primary_generator;
        this->secondary = secondary_generator;
    }

    ~Concat_Generator() {
        delete primary;
        delete secondary;
    }

    // Generator<T>* clone(Lazy_Sequence<T>* new_owner) override {

    // }

    T get_next() override {
        if (!primary->has_next()) return secondary->get_next();
        return primary->get_next();
    }

    bool has_next() override {
        return primary->has_next() || secondary->has_next();
    }
};