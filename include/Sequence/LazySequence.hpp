#pragma once

#include "Generator.hpp"
#include "Sequence.hpp"
#include "VectorSequence.hpp"
#include <iostream> 
#include <functional> 

template <typename T>
class LazySequence 
{
private:
    Generator<T>* generator;
    Sequence<T>* cache;

public:
    
    LazySequence(Sequence<T>*  seq, size_t arity, std::function<T(Sequence<T>*)> rule){
        cache = seq;
        generator = new Generator<T>(this, arity, rule);
    }

    ~LazySequence() {
        delete cache;
        delete generator;
    }

    T get(int index) {
        while (cache->get_size() <= index) {
            cache->append(generator->get_next());
        }
        return cache->get(index);
    }

    size_t get_materialized_count() {
        return cache->get_size();
    }
    
    std::string to_string() {
        return cache->to_string();
    }

};
