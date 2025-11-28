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
    Unique_Ptr<Generator<T>> generator;
    Unique_Ptr<Sequence<T>> materialized_data;

public:

    Lazy_Sequence(Sequence<T>*  start_sequence, size_t arity, std::function<T(Sequence<T>*)> rule){ 
        this->materialized_data = Unique_Ptr<Sequence<T>>(start_sequence);
        this->generator = make_unique<Function_Generator<T>>(this, arity, rule);
    }

    Lazy_Sequence(Sequence<T>* start_sequence, Generator<T>* gen) {
        this->materialized_data = Unique_Ptr<Sequence<T>>(start_sequence);
        this->generator = Unique_Ptr<Generator<T>>(gen);
    }

    Lazy_Sequence(Sequence<T>*  sequence){
        this->materialized_data = make_unique<Array_Sequence<T>>();
        this->generator = make_unique<Sequence_Generator<T>>(sequence);
    }

    ~Lazy_Sequence() {
        // delete materialized_data;
        // delete generator;
    }

    T get(int index) {
        while (generator->has_next() && materialized_data->get_size() <= index) {
            materialized_data->append(generator->get_next());
        }

        if (materialized_data->get_size() < index)
            throw std::runtime_error("Index beyond possible generation");

        return materialized_data->get(index);
    }

    T get_first() const {
        return materialized_data->get_first();
    }

    T get_last() const {
        return materialized_data->get_last();
    }

    bool has_next() const {
        return generator->has_next();
    }

    // Lazy_Sequence<T>* get_subsequence(int start_index, int end_index) {
    //     Lazy_Sequence<T>* sub_lazy_seq = new Lazy_Sequence<T>(
    //         materialized_data->get_subsequence(start_index, end_index),
    //         generator->clone(sub_lazy_seq)
    //     );

    //     return sub_lazy_seq;
    // }

    Lazy_Sequence<T>* append(Sequence<T>* items) { //lazy_seq можно аппендить!
        auto end_generator = make_unique<Sequence_Generator<T>>(items);
        auto append_generator = make_unique<Concat_Generator<T>>(std::move(this->generator), std::move(end_generator));
        this->generator = std::move(append_generator);

        return this;
    }

    Lazy_Sequence<T>* prepend(Sequence<T>* items) {
        this->materialized_data->reset();
        auto start_generator = make_unique<Sequence_Generator<T>>(items);
        auto prepend_generator = make_unique<Concat_Generator<T>>(std::move(start_generator), std::move(this->generator));
        this->generator = std::move(prepend_generator);

        return this;
    }

    //как можно расширять наобор операции на sequnce без изменнения seq?(для каждого изм. методв должен быть такой же генер)
    //ответ: вынести операции вне lazy_seq
    Lazy_Sequence<T>* insert_at(int insert_index, Sequence<T>* items) { 
        if (materialized_data->get_size() - 1 >= insert_index) {
            materialized_data->insert_at(insert_index, items);
            return this;
        }

        auto mid_generator = make_unique<Sequence_Generator<T>>(items);
        auto insert_generator = make_unique<Insert_Generator<T>>(
            std::move(this->generator), std::move(mid_generator),
            insert_index, materialized_data->get_size() - 1
        );

        this->generator = std::move(insert_generator);

        return this;
    }

    size_t get_materialized_count() const {
        return materialized_data->get_size();
    }
    
    std::string to_string() {
        return materialized_data->to_string();
    }

};
