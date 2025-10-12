#pragma once

#include<vector>
#include<functional>

template <typename T>
class Generator
{
private: 
    std::function<T(int)> rule;

public:
    Generator(std::function<T(int)> r) : rule(r) {};

    T Get(int index) {
        return rule(index);
    }

};