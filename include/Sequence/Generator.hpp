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
    Shared_Ptr<Lazy_Sequence<T>> first;
    Shared_Ptr<Lazy_Sequence<T>> second;

    size_t first_index;
    size_t second_index;

    bool first_is_empty;
    bool second_is_empty;

private:

    bool is_border_reached(const Shared_Ptr<Lazy_Sequence<T>>& seq, size_t index, bool& is_empty) {
        if (!is_empty && index >= seq->get_materialized_count()) {
            is_empty = true;
            return true;
        }
        return is_empty;
    }


public:
    Concat_Generator(Shared_Ptr<Lazy_Sequence<T>> first_seq, Shared_Ptr<Lazy_Sequence<T>> second_seq) {
        this->first = first_seq;
        this->second = second_seq;

        this->first_index = 0;
        this->second_index = 0;

        this->first_is_empty = false;
        this->second_is_empty = false;
    }

    ~Concat_Generator() {}

    T get_next() override {
        if (!is_border_reached(first, first_index, first_is_empty)) 
            return first->get(first_index++);
        if (first->has_next()) return first->get_next();
 
        if (!is_border_reached(second, second_index, second_is_empty)) 
            return second->get(second_index++);
        if (second->has_next()) return second->get_next();

        throw std::runtime_error("Generation limit reached");
    }

    bool has_next() override {
        return first->has_next() || second->has_next();
    }
};

template <typename T>
class Insert_Generator : public Generator<T> 
{
private:
    Shared_Ptr<Lazy_Sequence<T>> primary;
    Shared_Ptr<Lazy_Sequence<T>> secondary;

    size_t current_index;
    size_t insert_index;

    size_t primary_index;
    size_t secondary_index;

    bool primary_is_empty;
    bool secondary_is_empty;

private:
    bool is_border_reached(const Shared_Ptr<Lazy_Sequence<T>>& seq, size_t index, bool& is_empty) {
        if (!is_empty && index >= seq->get_materialized_count()) {
            is_empty = true;
            return true;
        }
        return is_empty;
    }

public:
    Insert_Generator(Shared_Ptr<Lazy_Sequence<T>> primary_seq, Shared_Ptr<Lazy_Sequence<T>> secondary_seq, 
        size_t insert_index) 
    {
        this->primary = primary_seq;
        this->secondary = secondary_seq;
        
        this->insert_index = insert_index;
        this->current_index = 0;
        this->primary_index = 0;
        this->secondary_index = 0;

        this->primary_is_empty = false;
        this->secondary_is_empty = false;
    }

    ~Insert_Generator() {}

    T get_next() override {
        if ((current_index >= insert_index - 1) && (secondary->has_next() || !secondary_is_empty)) {
            current_index++;
            if (!is_border_reached(secondary, secondary_index, secondary_is_empty)) 
                return secondary->get(secondary_index++);
            return secondary->get_next();
        }
        
        current_index++;
        if (!is_border_reached(primary, primary_index, primary_is_empty)) 
            return primary->get(primary_index++);
        return primary->get_next(); 
    }

    bool has_next() override {
        return primary->has_next() || secondary->has_next();
    }
};