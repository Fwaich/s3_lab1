#pragma once
#include "BinaryTree.hpp"
#include <stdexcept>
#include <optional>

template<typename T>
class Running_Median {
private:
    BinaryTree<T> tree;
    mutable std::optional<double> cached_median;
    mutable bool is_cache_valid = false;
  
private:
    void invalidate_cache() {
        is_cache_valid = false;
        cached_median.reset();
    }
    
    double compute_median() const {
        if (!is_cache_valid) {
            auto it = tree.iterator();
            size_t size = it.get_size();
            
            if (size == 0) {
                throw std::runtime_error("Cannot calculate median: no elements");
            }
            
            if (size % 2 == 1) {
                cached_median = static_cast<double>(it.get_at(size / 2));
            } else {
                T mid1 = it.get_at(size / 2 - 1);
                T mid2 = it.get_at(size / 2);
                cached_median = (static_cast<double>(mid1) + static_cast<double>(mid2)) / 2.0;
            }
            
            is_cache_valid = true;
        }
        
        return cached_median.value();
    }
    
public:
    Running_Median() = default;
    
    void add(const T& value) {
        tree.add(value);
        invalidate_cache();
    }
    
    double get_median() const {
        return compute_median();
    }
    
    void clear() {
        tree = BinaryTree<T>();
        invalidate_cache();
    }
    
};