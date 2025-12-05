#pragma once

#include "Generator.hpp"
#include "Sequence.hpp"
#include "ArraySequence.hpp"
#include "Cardinal.hpp"
#include "UniquePtr.hpp"
#include "SharedPtr.hpp"
#include <functional> 

template <typename T>
class Lazy_Sequence 
{
private:
    Unique_Ptr<Generator<T>> generator;
    Unique_Ptr<Array_Sequence<T>> materialized_data;

public:

    Lazy_Sequence(const Sequence<T>&  start_sequence, std::function<T(const Array_Sequence<T>&)> rule){ 
        this->materialized_data = make_unique<Array_Sequence<T>>();
        this->generator =  ::make_unique<Function_Generator<T>>(start_sequence, rule);
    }

    Lazy_Sequence(const Sequence<T>& sequence){
        this->materialized_data = make_unique<Array_Sequence<T>>();
        this->generator = make_unique<Sequence_Generator<T>>(sequence);
    }

    Lazy_Sequence(Unique_Ptr<Generator<T>>&& gen) {
        this->materialized_data = make_unique<Array_Sequence<T>>();
        this->generator = std::move(gen);
    }

    T get(size_t index) {
        while (generator->has_next() && materialized_data->get_size() <= index) {
            materialized_data->append(generator->get_next());
        }

        if (materialized_data->get_size() < index)
            throw std::runtime_error("Index beyond possible generation");

        return materialized_data->get(index);
    }

    T get_first_materialized() const {
        return materialized_data->get_first();
    }

    T get_last_materialized() const {
        return materialized_data->get_last();
    }

    bool has_next() const {
        return generator->has_next();
    }

    Lazy_Sequence<T> append(const Lazy_Sequence<T>& items) {
        auto append_generator = make_unique<Concat_Generator<T>>(
            this->generator->clone(), items.generator->clone()
        );  
        return Lazy_Sequence<T>(std::move(append_generator)); 
    }

    Lazy_Sequence<T> prepend(const Lazy_Sequence<T>& items) {
        auto prepend_generator = make_unique<Concat_Generator<T>>(
            items.generator->clone(), this->generator->clone()
        );  
        return Lazy_Sequence<T>(std::move(prepend_generator)); 
    }

    Lazy_Sequence<T> insert_at(int insert_index, const Lazy_Sequence<T>& items) { 
        auto insert_generator = make_unique<Insert_Generator<T>>(
            std::move(this->generator), std::move(items.generator->clone()),
            insert_index, materialized_data->get_size() - 1
        );

        return Lazy_Sequence<T>(std::move(insert_generator));
    }

    size_t get_materialized_count() const {
        return materialized_data->get_size();
    }
    
    std::string to_string() {
        return materialized_data->to_string();
    }

};
