#include "Sequence.h"
#include "ArraySequence.h"
#include <iostream>


int main() {
    ArraySequence<int> a;

    a.append(1);
    std::cout << a.to_string() << std::endl; 

    return 0;
}