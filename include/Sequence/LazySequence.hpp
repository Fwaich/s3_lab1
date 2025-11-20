#pragma once

#include "Generator.hpp"
#include "Sequence.hpp"
#include "VectorSequence.hpp"
#include "Cardinal.hpp"
#include "UniquePtr.hpp"
#include <functional> 

template <typename T>
class Lazy_Sequence 
{
private:
    Generator<T>* generator;
    Sequence<T>* seq;

public:

    Lazy_Sequence(Sequence<T>*  seq, size_t arity, std::function<T(Sequence<T>*)> rule){
        this->seq = seq;
        this->generator = new Function_Generator<T>(this, arity, rule);
    }

    Lazy_Sequence(Sequence<T>* seq, Generator<T>* gen) {
        this->seq = seq;
        this->generator = gen;
    }

    Lazy_Sequence(Sequence<T>*  seq){
        this->seq = new Vector_Sequence<T>; //  будет заполнять из статика в seq lazy_sequence'a
        this->generator = new Sequence_Generator(seq);
    }

    ~Lazy_Sequence() {
        delete seq;
        delete generator;
    }

    T get(int index) {
        while (generator->has_next() && seq->get_size() <= index) {
            seq->append(generator->get_next());
        }
        return seq->get(index);
    }

    T get_first() {
        return seq->get_first();
    }

    T get_last() {
        return seq->get_last();
    }

    bool has_next() {
        return generator->has_next();
    }

    // Lazy_Sequence<T>* get_subsequence(int start_index, int end_index) {
    //     Lazy_Sequence<T>* sub_lazy_seq = new Lazy_Sequence<T>(
    //         seq->get_subsequence(start_index, end_index),
    //         generator->clone(sub_lazy_seq)
    //     );

    //     return sub_lazy_seq;
    // }

    Lazy_Sequence<T>* append(Sequence<T>* items) {
        this->seq->reset();
        Generator<T>* end_generator = new Sequence_Generator<T>(items);
        Generator<T>* append_generator = new Concat_Generator(this->generator, end_generator);
        this->generator = append_generator;

        return this;
    }

    Lazy_Sequence<T>* prepend(Sequence<T>* items) {
        this->seq->reset();
        Generator<T>* start_generator = new Sequence_Generator<T>(items);
        Generator<T>* prepend_generator = new Concat_Generator(start_generator, this->generator);
        this->generator = prepend_generator;

        return this;
    }

    size_t get_materialized_count() {
        return seq->get_size();
    }
    
    std::string to_string() {
        return seq->to_string();
    }

};
