#pragma once

#include "Generator.hpp"
#include "Sequence.hpp"
#include "ArraySequence.hpp"
#include "Cardinal.hpp"
#include "UniquePtr.hpp"
#include <functional> 

template <typename T>
class Lazy_Sequence 
{
private:
    Generator<T>* generator;
    Sequence<T>* sequence; //MATERIALIZED

public:

    Lazy_Sequence(Sequence<T>*  sequence, size_t arity, std::function<T(Sequence<T>*)> rule){ 
        this->sequence = sequence; //start_seq and materialized
        this->generator = new Function_Generator<T>(this, arity, rule);
    }

    Lazy_Sequence(Sequence<T>* sequence, Generator<T>* gen) {
        this->sequence = sequence;
        this->generator = gen;
    }

    Lazy_Sequence(Sequence<T>*  sequence){
        this->sequence = new Array_Sequence<T>;
        this->generator = new Sequence_Generator(sequence);
    }

    ~Lazy_Sequence() {
        delete sequence;
        delete generator;
    }

    T get(int index) {
        while (generator->has_next() && sequence->get_size() <= index) {
            sequence->append(generator->get_next());
        }
        return sequence->get(index); //упорядочить проверки has_next()
    }

    T get_first() {
        return sequence->get_first();
    }

    T get_last() {
        return sequence->get_last();
    }

    bool has_next() {
        return generator->has_next();
    }

    // Lazy_Sequence<T>* get_subsequence(int start_index, int end_index) {
    //     Lazy_Sequence<T>* sub_lazy_seq = new Lazy_Sequence<T>(
    //         sequence->get_subsequence(start_index, end_index),
    //         generator->clone(sub_lazy_seq)
    //     );

    //     return sub_lazy_seq;
    // }

    Lazy_Sequence<T>* append(Sequence<T>* items) { //lazy_seq можно аппендить!
        Generator<T>* end_generator = new Sequence_Generator<T>(items);
        Generator<T>* append_generator = new Concat_Generator(this->generator, end_generator);
        this->generator = append_generator;

        return this;
    }

    Lazy_Sequence<T>* prepend(Sequence<T>* items) {
        this->sequence->reset();
        Generator<T>* start_generator = new Sequence_Generator<T>(items);
        Generator<T>* prepend_generator = new Concat_Generator(start_generator, this->generator);
        this->generator = prepend_generator;

        return this;
    }

    //как можно расширять наобор операции на sequnce без изменнения seq?(для каждого изм. методв должен быть такой же генер)
    //ответ: вынести операции вне lazy_seq
    Lazy_Sequence<T>* insert_at(int insert_index, Sequence<T>* items) { 
        if (sequence->get_size() - 1 >= insert_index) {
            sequence->insert_at(insert_index, items);
            return this;
        }

        Generator<T>* mid_generator = new Sequence_Generator<T>(items);
        Generator<T>* insert_generator = new Insert_Generator(this->generator, mid_generator , insert_index, sequence->get_size() - 1);
        this->generator = insert_generator;

        return this;
    }

    size_t get_materialized_count() {
        return sequence->get_size();
    }
    
    std::string to_string() {
        return sequence->to_string();
    }

};
