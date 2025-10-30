#include "Sequnces.hpp"
#include "UniquePtr.hpp"
#include "ReadOnlyStream.hpp"
#include <iostream>
#include <memory>

auto fib = [](Sequence<long long>* seq) -> long long {
    size_t size = seq->get_size();
    return seq->get(size - 1) + seq->get(size - 2);
};

void test_copy_sequence() {
    Sequence<int>* a = new ArraySequence<int>;
    for (int i = 0; i < 100; ++i) {
        a->append(i);
    } 

    VectorSequence<int> vec(a);
    

    delete a;
}

void test_lazy_seq() {
    Sequence<long long>* a = new VectorSequence<long long>;
    for (int i = 0; i < 2; ++i) {
        a->append(i);
    }

    LazySequence<long long> l(a, 2, fib);

    for (int i = 0; i < 1000000; i++) {
        long long item = l.get(i);
    }
    
}





int main() {
    try {

        // test_copy_sequence();
        // test_operator();
        test_lazy_seq();

    } catch(const out_of_range& e){
        std::cout << e.what() << std::endl; 
    }  catch(const data_is_null& e){
        std::cout << e.what() << std::endl; 
    }

    return 0;
}