#pragma once

#include "Generator.hpp"
#include "Sequence.hpp"
#include "ArraySequence.hpp"
#include "Cardinal.hpp"
#include "UniquePtr.hpp"
#include "SharedPtr.hpp"
#include <functional> 

template <typename T>
class Lazy_Sequence : public Enable_Shared_From_This<Lazy_Sequence<T>>
{
private:
    Shared_Ptr<Generator<T>> generator;
    Unique_Ptr<Array_Sequence<T>> materialized_data;

private:
    void init_generator(size_t arity, std::function<T(const Array_Sequence<T>&)> rule) {
        generator = ::make_shared<Function_Generator<T>>(
            this->shared_from_this(), arity, rule
        );
    }
   
public:
    Lazy_Sequence(const Sequence<T>&  start_sequence, size_t arity, std::function<T(const Array_Sequence<T>&)> rule)
        : materialized_data(make_unique<Array_Sequence<T>>(start_sequence)) {}

    Lazy_Sequence(const Sequence<T>& sequence){
        this->materialized_data = make_unique<Array_Sequence<T>>();
        this->generator = make_shared<Sequence_Generator<T>>(sequence);
    }

    Lazy_Sequence(Shared_Ptr<Generator<T>> gen) {
        this->materialized_data = make_unique<Array_Sequence<T>>();
        this->generator = gen;
    }
    
    static Shared_Ptr<Lazy_Sequence<T>> create(const Sequence<T>&  start_sequence,
         size_t arity, std::function<T(const Array_Sequence<T>&)> rule) 
    {
        auto l = Shared_Ptr<Lazy_Sequence<T>>(
            new Lazy_Sequence<T>(start_sequence, arity, rule)
        );
        l->init_generator(arity, rule);
        return l;
    }

    static Shared_Ptr<Lazy_Sequence<T>> create(const Sequence<T>& sequence) 
    {
        return Shared_Ptr<Lazy_Sequence<T>>(
            new Lazy_Sequence<T>(sequence)
        );
    }

    // Lazy_Sequence(Unique_Ptr<Generator<T>>&& gen) {
    //     this->materialized_data = make_unique<Array_Sequence<T>>();
    //     this->generator = std::move(gen);
    // }
    
    T get(size_t index) {
        while (generator->has_next() && materialized_data->get_size() <= index) {
            materialized_data->append(generator->get_next());
        }

        if (materialized_data->get_size() < index)
            throw std::runtime_error("Index beyond possible generation");

        return materialized_data->get(index);
    }

    T get_next() {
        return this->get(materialized_data->get_size());
    }

    T get_first_materialized() const {
        return materialized_data->get_first();
    }

    T get_last_materialized() const {
        return materialized_data->get_last();
    }

    size_t get_materialized_count() const {
        return materialized_data->get_size();
    }

    bool has_next() const {
        return generator->has_next();
    }

    Shared_Ptr<Lazy_Sequence<T>> append(Shared_Ptr<Lazy_Sequence<T>> items) {
        auto append_generator = make_shared<Concat_Generator<T>>(
            this->shared_from_this(), items
        );  
        return make_shared<Lazy_Sequence<T>>(append_generator); 
    }

    Shared_Ptr<Lazy_Sequence<T>> prepend(Shared_Ptr<Lazy_Sequence<T>> items) {
        auto prepend = make_shared<Concat_Generator<T>>(
            items, this->shared_from_this()
        );  
        return make_shared<Lazy_Sequence<T>>(prepend); 
    }

    Shared_Ptr<Lazy_Sequence<T>> insert_at(size_t insert_index, Shared_Ptr<Lazy_Sequence<T>>items) { 
        auto insert_generator = make_shared<Insert_Generator<T>>(
            this->shared_from_this(), items,
            insert_index
        );

        return make_shared<Lazy_Sequence<T>>(insert_generator);
    }

    Shared_Ptr<Lazy_Sequence<T>> get_subsequence(size_t from_index, size_t to_index) { 
        auto subsequence_generator = make_shared<Subsequence_Generator<T>>(
            this->shared_from_this(),
            from_index, to_index
        );

        return make_shared<Lazy_Sequence<T>>(subsequence_generator);
    }

    template <typename T2>
    Shared_Ptr<Lazy_Sequence<T>> map(std::function<T2(T)> func) { 
        auto map_generator = ::make_shared<Map_Generator<T2, T>>(
            this->shared_from_this(),
            func
        );

        return make_shared<Lazy_Sequence<T2>>(map_generator);
    }

    Shared_Ptr<Lazy_Sequence<T>> where(std::function<bool(T)> func) { 
        auto where_generator = ::make_shared<Where_Generator<T>>(
            this->shared_from_this(),
            func
        );

        return make_shared<Lazy_Sequence<T>>(where_generator);
    }

};
