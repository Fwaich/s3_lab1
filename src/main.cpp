#include "Sequence.h"
#include "ArraySequence.h"
#include <iostream>


int main() {
    ArraySequence<int> a;

    for (int i = 0; i < 1000; i++) {
        a.append(1);
    }
    
    std::cout << a.to_string() << std::endl; 
    std::cout << a.get_size() << std::endl; 

    return 0;
}