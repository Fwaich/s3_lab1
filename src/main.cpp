#include "Sequnces.hpp"
#include "UniquePtr.hpp"
#include "ReadOnlyStream.hpp"
#include "Cardinal.hpp"
#include <iostream>
#include <memory>


auto double_it = [](int index) -> int {
    return index * 2;
};

auto fib = [](Sequence<long long>* seq) -> long long {
    size_t size = seq->get_size();
    return seq->get(size - 1) + seq->get(size - 2);
};

long long fib1 (Sequence<long long>* seq) {
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

    auto fib1_ptr = fib1;
    Sequence<long long>* a = new VectorSequence<long long>;
    for (int i = 0; i < 2; ++i) {
        a->append(i);
    }

    LazySequence<long long> l(a, 2, fib1);

    for (int i = 0; i <= 10; i++) {
        long long item = l.get(i);
        std::cout << item << std::endl;
    }

    // LazySequence<long long> l(a, double_it);

    // for (int i = 0; i <= 10; i++) {
    //     long long item = l.get(i);
    //     std::cout << item << std::endl;
    // }

    // std::cout << l.get(1) << std::endl;

    auto sub = l.get_subsequence(5, 10);
    std::cout << sub->to_string() << std::endl;

    for (int i = 0; i < 20; i++) {
        long long item = sub->get(i);
        std::cout << item << std::endl;
    }

    delete sub;
    
}

void test_cardinal() {

    Cardinal val = 0;
    if (!val.is_infinite()) {
        std::cout << val.get_value() << std::endl;
    }

    // std::cout << val << std::endl;
    
}





int main() {
    try {

        // test_copy_sequence();
        // test_operator();
        test_lazy_seq();
        // test_cardinal();

    } catch(const out_of_range& e){
        std::cout << e.what() << std::endl; 
    }  catch(const data_is_null& e){
        std::cout << e.what() << std::endl; 
    }

    return 0;
}