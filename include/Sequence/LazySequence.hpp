#pragma once

#include "Generator.hpp"
#include "Sequence.hpp"
#include "VectorSequence.hpp"
#include "Cardinal.hpp"
#include <functional> 

template <typename T>
class LazySequence 
{
private:
    Generator<T>* generator;
    Sequence<T>* seq;

public:
    
    LazySequence(Sequence<T>*  seq, std::function<T(int)> rule){
        this->seq = seq;
        this->generator = new Index_Generator<T>(this, seq->get_size(), rule);
    }

    LazySequence(Sequence<T>*  seq, size_t arity, std::function<T(Sequence<T>*)> rule){
        this->seq = seq;
        this->generator = new Sequence_Generator<T>(this, arity, rule);
    }

    LazySequence(Sequence<T>*  seq){
        this->seq = seq;
        this->generator = nullptr;
    }

    ~LazySequence() {
        delete seq;
        delete generator;
    }

    T get(int index) {
        if (!generator) return seq->get(index);

        while (seq->get_size() <= index) {
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

    LazySequence<T>* get_subsequence(int start_index, int end_index) {
        LazySequence<T>* sub_lazy_seq = new LazySequence<T>(seq->get_subsequence(start_index, end_index));
        if (!this->generator) return sub_lazy_seq;

        sub_lazy_seq->generator = this->generator->clone(sub_lazy_seq);
        return sub_lazy_seq;
    }

    LazySequence<T>* append(T item) {
        
    }

    size_t get_materialized_count() {
        return seq->get_size();
    }
    
    std::string to_string() {
        return seq->to_string();
    }

};
