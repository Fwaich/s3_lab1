#pragma once
#include"UniquePtr.hpp"

template <typename T>
class Lazy_Sequence;

template <typename T>
class Generator
{
public:
    virtual T get_next() = 0;
    virtual bool has_next() = 0;
    virtual Unique_Ptr<Generator<T>> clone() const = 0;
    virtual ~Generator() = default;
};

template <typename T>
class Function_Generator : public Generator<T>
{
private: 
    Array_Sequence<T> start_sequence;
    Array_Sequence<T> args_buffer; //Ring_Buffer
    std::function<T(const Sequence<T>&)> rule;
    size_t current_index;

public:
    Function_Generator(const Sequence<T>& start_seq, std::function<T(const Array_Sequence<T>&)> rule)
    : start_sequence(start_seq), args_buffer(start_seq) {

        this->current_index = 0;
        this->rule = rule;
    }

    T get_next() override {
        if (current_index < start_sequence.get_size())
            return start_sequence.get(current_index++);

        T item = rule(args_buffer);
        args_buffer.remove(0);
        args_buffer.append(item);
        current_index++;

        return item;
    }

    Unique_Ptr<Generator<T>> clone() const override {
        return Unique_Ptr<Generator<T>>(
            new Function_Generator<T>(start_sequence, rule)
        );
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
    Sequence_Generator(const Sequence<T>& seq) : sequence(seq), current_index(0) {} 

    Sequence_Generator(const Array_Sequence<T>& seq) : sequence(seq), current_index(0) {} 

    T get_next() override {
        return sequence.get(current_index++);
    }

    Unique_Ptr<Generator<T>> clone() const override {
        return Unique_Ptr<Generator<T>>(
            new Sequence_Generator<T>(this->sequence)
        );
    }

    bool has_next() override {
        return current_index < sequence.get_size();
    }
};

template <typename T>
class Concat_Generator : public Generator<T> 
{
private:
    Unique_Ptr<Generator<T>> first;
    Unique_Ptr<Generator<T>> second;

public:
    Concat_Generator(Unique_Ptr<Generator<T>>&& first_generator, Unique_Ptr<Generator<T>>&& second_generator) {
        this->first = std::move(first_generator);
        this->second = std::move(second_generator);
    }


    T get_next() override {
        if (!first->has_next()) return second->get_next();
        return first->get_next();
    }

    Unique_Ptr<Generator<T>> clone() const override {
        return Unique_Ptr<Generator<T>>(
            new Concat_Generator<T>(first->clone(), second->clone())
        );
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

    T get_next() override {
        if (current_index >= insert_index - 1 && secondary->has_next()) {
            current_index++;
            return secondary->get_next();
        }
        
        current_index++;
        return primary->get_next(); 
    }

    Unique_Ptr<Generator<T>> clone() const override {
        return Unique_Ptr<Generator<T>>(
            new Insert_Generator<T>(primary->clone(), secondary->clone(), insert_index, 0)
        );
    }

    bool has_next() override {
        return primary->has_next() || secondary->has_next();
    }
};