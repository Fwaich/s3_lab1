#pragma once

#include "Sequence.hpp"
#include "LazySequence.hpp"
#include <exception>

template <typename T>
class Read_Only_Stream {
public:
    virtual ~Read_Only_Stream() = default;

    virtual bool is_end_of_stream() const = 0;
    virtual bool is_can_seek() const = 0; // is_ подряд плохо читаетсяя
    virtual bool seek(size_t index) = 0;

    virtual T read() = 0;

    virtual void reset() = 0;

    virtual size_t get_position() const = 0;
    virtual size_t get_size() const = 0;
};

template <typename T>
class Lazy_Read_Only_Stream : public Read_Only_Stream<T> //генератор коорый читает поток
{
private:
    Shared_Ptr<Lazy_Sequence<T>> lazy_sequence;
    size_t position;

public:

    ~Lazy_Read_Only_Stream() {}

    Lazy_Read_Only_Stream(Shared_Ptr<Lazy_Sequence<T>> lazy_seq) 
        : lazy_sequence(lazy_seq), position(0) {}

    bool is_end_of_stream() const override {
        return !lazy_sequence->has_next();
    }

    bool is_can_seek() const override {
        return true;
    }

    T read() {
        return lazy_sequence->get(position++);
    }

    bool seek(size_t index) override { 
        if (!this->is_can_seek()) return false; 

        if (index < 0 || index >= lazy_sequence->get_materialized_count()) 
            throw std::runtime_error("Index beyond matrialized data");

        position = index;
        return true; 
    }

    void reset() override {
        position = 0;
    }

    size_t get_position() const override {
        return position;
    }

    size_t get_size() const override {
        return lazy_sequence->get_materialized_count();
    }

};

// template <typename T>
// class Sequence_Read_Only_Stream 
// {
// private:
//     Sequence<T>* data;
//     size_t pos;

// public:

//     Read_Only_Stream(Sequence<T>* seq) : data(seq), pos(0) {};

//     ~Read_Only_Stream() {
//         delete data;
//     }

//     bool is_end_off_stream() const {
//         return pos >= data->get_size();
//     }

//     T  read() {
//         if (this->is_end_off_stream()) {
//             throw out_of_range();
//         }
//         return data->get(pos++);
//     }

//     size_t get_position() const {
//         return pos;
//     }

//     void seek(size_t index) {
//         if (index >= data->get_size() || index < 0) {
//             throw out_of_range();
//         }

//         pos = index;
//     }

//     void reset() {
//         pos = 0;
//     }


//     size_t size() {
//         return data->get_size();
//     }

// };