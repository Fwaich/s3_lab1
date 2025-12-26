#pragma once
#include"UniquePtr.hpp"
#include"SharedPtr.hpp"
#include"WeakPtr.hpp"

template <typename T>
class Lazy_Sequence;

template <typename T>
class Generator
{
public:
    virtual T get_next() = 0;
    virtual bool has_next() = 0;
    virtual ~Generator() = default;
};

template <typename T>
class Function_Generator : public Generator<T>
{
private: 
    Weak_Ptr<Lazy_Sequence<T>> owner;
    std::function<T(const Sequence<T>&)> rule;
    size_t arity;

public:
    Function_Generator(Shared_Ptr<Lazy_Sequence<T>> gen_owner, size_t arity, std::function<T(const Sequence<T>&)> rule) 
        : owner(gen_owner)
    {
        this->arity = arity;
        this->rule = rule;
    }
    
    ~Function_Generator() {}

    T get_next() override {
        if (owner->get_materialized_count() <  arity)
            throw std::runtime_error("Not enough elements to generate next");

        Array_Sequence<T> args_buffer(arity);
        size_t size = owner->get_materialized_count();
        for (size_t i = 0; i < args_buffer.get_size(); i++) {
            args_buffer.set(i, owner->get(size - 1 - i));
        }
        T item = rule(args_buffer);
        return item;
    }

    bool has_next() {
        return true;
    }

};

template <typename T>
class Sequence_Generator : public Generator<T> 
{
private:
    Array_Sequence<T> sequence;
    size_t current_index;

public:
    Sequence_Generator(const Array_Sequence<T>& seq)
        : sequence(seq)
    { 
        this->current_index = 0;
    } 

    Sequence_Generator(const Array_Sequence<T>& seq, size_t current_index)
        : sequence(seq)
    { 
        this->current_index = current_index;
    } 

    ~Sequence_Generator() {}

    T get_next() override {
        return sequence.get(current_index++);
    }

    bool has_next() override {
        return current_index < sequence.get_size();
    }
};

template <typename T>
class Concat_Generator : public Generator<T> 
{
private:
    Shared_Ptr<Generator<T>> first;
    Shared_Ptr<Generator<T>> second;

public:
    Concat_Generator(Shared_Ptr<Generator<T>> first_generator, Shared_Ptr<Generator<T>> second_generator) {
        this->first = first_generator;
        this->second = second_generator;
    }

    ~Concat_Generator() {
        
    }

    T get_next() override {
        if (!first->has_next()) return second->get_next();
        return first->get_next();
    }

    bool has_next() override {
        return first->has_next() || second->has_next();
    }
};

template <typename T>
class Insert_Generator : public Generator<T> 
{
private:
    Unique_Ptr<Generator<T>> primary;
    Unique_Ptr<Generator<T>> secondary;
    int current_index;
    int insert_index;

public:
    Insert_Generator(Unique_Ptr<Generator<T>>&& primary_generator, Unique_Ptr<Generator<T>>&& secondary_generator, 
        int insert_index, int current_index) {

        this->primary = std::move(primary_generator);
        this->secondary = std::move(secondary_generator);
        this->insert_index = insert_index;
        this->current_index = current_index;
    }

    ~Insert_Generator() {
        
    }

    T get_next() override {
        if (current_index >= insert_index - 1 && secondary->has_next()) {
            current_index++;
            return secondary->get_next();
        }
        
        current_index++;
        return primary->get_next(); 
    }

    bool has_next() override {
        return primary->has_next() || secondary->has_next();
    }
};