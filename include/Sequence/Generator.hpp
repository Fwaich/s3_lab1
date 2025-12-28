#pragma once
#include <optional>
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
        for (size_t i = 0; i < arity; i++) {
            args_buffer.set(i, owner->get(size - arity + i));
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
    Shared_Ptr<Lazy_Sequence<T>> first;
    Shared_Ptr<Lazy_Sequence<T>> second;

    size_t first_index;
    size_t second_index;

public:
    Concat_Generator(Shared_Ptr<Lazy_Sequence<T>> first_seq, Shared_Ptr<Lazy_Sequence<T>> second_seq) {
        this->first = first_seq;
        this->second = second_seq;

        this->first_index = 0;
        this->second_index = 0;
    }

    ~Concat_Generator() {}

    T get_next() override {
        bool can_use_first = first->has_next() || first_index < first->get_materialized_count();
        bool can_use_second = second->has_next() || second_index < second->get_materialized_count();

        if (can_use_first) 
            return first->get(first_index++);
 
        if (can_use_second) 
            return second->get(second_index++);

        throw std::runtime_error("Generation limit reached");
    }

    bool has_next() override {
        bool can_use_first = first->has_next() || first_index < first->get_materialized_count();
        bool can_use_second = second->has_next() || second_index < second->get_materialized_count();
        return can_use_first || can_use_second;
    }
};

template <typename T>
class Insert_Generator : public Generator<T> 
{
private:
    Shared_Ptr<Lazy_Sequence<T>> primary;
    Shared_Ptr<Lazy_Sequence<T>> secondary;

    size_t primary_index;
    size_t secondary_index;

    size_t current_index;
    size_t insert_index;

public:
    Insert_Generator(Shared_Ptr<Lazy_Sequence<T>> primary_seq, Shared_Ptr<Lazy_Sequence<T>> secondary_seq, 
        size_t insert_index) 
    {
        this->primary = primary_seq;
        this->secondary = secondary_seq;
        
        this->primary_index = 0;
        this->secondary_index = 0;

        this->insert_index = insert_index;
        this->current_index = 0;
    }

    ~Insert_Generator() {}

    T get_next() override {
        bool can_use_primary = primary->has_next() || primary_index < primary->get_materialized_count();
        bool can_use_secondary = secondary->has_next() || secondary_index < secondary->get_materialized_count();

        if (current_index >= insert_index - 1 && can_use_secondary) {
            current_index++;
            return secondary->get(secondary_index++);
        }
        
        current_index++;
        if (can_use_primary) 
            return primary->get(primary_index++);

        throw std::runtime_error("Generation limit reached");
    }

    bool has_next() override {
        bool can_use_primary = primary->has_next() || primary_index < primary->get_materialized_count();
        bool can_use_secondary = secondary->has_next() || secondary_index < secondary->get_materialized_count();
        return can_use_primary || can_use_secondary;
    }
};

template <typename T>
class Subsequence_Generator : public Generator<T> 
{
private:
    Shared_Ptr<Lazy_Sequence<T>> sequence;

    size_t current_index;
    size_t from_index;
    size_t to_index;

public:
    Subsequence_Generator(Shared_Ptr<Lazy_Sequence<T>> seq, size_t from_index, size_t to_index) 
    {
        this->sequence = seq;

        this->current_index = from_index;
        this->from_index = from_index;
        this->to_index = to_index;
    }

    ~Subsequence_Generator() {}

    T get_next() override {
        if (this->has_next())
            return sequence->get(current_index++);

        throw std::runtime_error("Generation limit reached");
    }

    bool has_next() override {
        bool can_use_seq = sequence->has_next() || current_index < sequence->get_materialized_count();
        return (current_index >= from_index && current_index <= to_index) && can_use_seq;
    }
};

template <typename TOut, typename TIn>
class Map_Generator : public Generator<TOut> 
{
private:
    Shared_Ptr<Lazy_Sequence<TIn>> sequence;
    size_t current_index;

    std::function<TOut(TIn)> func;

public:
    Map_Generator(Shared_Ptr<Lazy_Sequence<TIn>> seq, std::function<TOut(TIn)> func) 
    {
        this->sequence = seq;
        this->current_index = 0;

        this->func = func;
    }

    ~Map_Generator() {}

    TOut get_next() override {
        if (this->has_next())
            return func(sequence->get(current_index++));

        throw std::runtime_error("Generation limit reached");
    }

    bool has_next() override {
        bool can_use_seq = sequence->has_next() || current_index < sequence->get_materialized_count();
        return can_use_seq;
    }
};

template <typename T>
class Where_Generator : public Generator<T> 
{
private:
    Shared_Ptr<Lazy_Sequence<T>> sequence;
    size_t current_index;

    std::optional<T> cached_item;
    std::function<bool(T)> func;

public:
    Where_Generator(Shared_Ptr<Lazy_Sequence<T>> seq, std::function<bool(T)> func) 
    {
        this->sequence = seq;
        this->current_index = 0;

        this->cached_item = std::nullopt;
        this->func = func;
    }
    
    T get_next() override {
        if (this->has_next()) {
            T result = *cached_item;
            cached_item.reset();
            return result;
        }
        
        throw std::runtime_error("Generation limit reached");
    }
    
    bool has_next() override {  //ограничить?
        if (cached_item.has_value())
            return true;

        while (sequence->has_next() || current_index < sequence->get_materialized_count()) {

            T item = sequence->get(current_index++);

            if (func(item)) {
                cached_item = item;
                return true;
            }
        }

        return false;
    }
};