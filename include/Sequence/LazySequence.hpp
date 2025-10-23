#pragma once

#include "Generator.hpp"
#include<iostream> 
#include<functional> 
#include<vector>

template <typename T>
class LazySequnce 
{
private:
    Generator<T> generator;
    std::vector<T> cache;
public:

    LazySequnce(std::function<T(int)> rule) : generator(rule) {};

    void PrintFirstN(int n) {
        int index = 1;
        while (cache.size() <= n) {
            cache.push_back(generator.Get(index));
            
            std::cout << cache[index - 1] << ' ';
            index++;
        }
        std::cout << std::endl;
    }

};
