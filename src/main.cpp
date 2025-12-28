#include "Sequnces.hpp"
#include "UniquePtr.hpp"
#include "SharedPtr.hpp"
#include "exceptions.hpp"
// #include "ReadOnlyStream.hpp"
#include "Cardinal.hpp"
#include <iostream>
#include <memory>


// auto double_it = [](int index) -> int {
//     return index * 2;
// };

auto fib = [](const Array_Sequence<int>& seq) -> int {
    size_t size = seq.get_size();
    return seq.get(size - 1) + seq.get(size - 2);
};

auto x2 = [](const Array_Sequence<int>& seq) -> int {
    size_t size = seq.get_size();
    return seq.get(size - 1) * 2;
};

std::function<int(int)> map_x2 = [](int item) -> int {
    return item*2;
};

std::function<bool(int)> where_div2 = [](int item) -> bool {
    if (item % 2 == 0) return true;

    return false;
};

// long long fib1 (Sequence<long long>* seq) {
//     size_t size = seq->get_size();
//     return seq->get(size - 1) + seq->get(size - 2);
// };

// void test_copy_sequence() {
//     Sequence<int>* a = new ArraySequence<int>;
//     for (int i = 0; i < 100; ++i) {
//         a->append(i);
//     } 

//     Array_Sequence<int> vec(a);
    

//     delete a;
// }

// void test_lazy_seq() {

//     auto fib1_ptr = fib1;
//     Sequence<long long>* a = new Array_Sequence<long long>;
//     for (int i = 0; i < 2; ++i) {
//         a->append(i);
//     }

//     Lazy_Sequence<long long> l(a, 2, fib1);

//     for (int i = 0; i <= 10; i++) {
//         long long item = l.get(i);
//         std::cout << item << std::endl;
//     }

//     // Lazy_Sequence<long long> l(a, double_it);

//     // for (int i = 0; i <= 10; i++) {
//     //     long long item = l.get(i);
//     //     std::cout << item << std::endl;
//     // }

//     // std::cout << l.get(1) << std::endl;

//     // auto sub = l.get_subsequence(5, 10);
//     // std::cout << sub->to_string() << std::endl;

//     // for (int i = 0; i < 20; i++) {
//     //     long long item = sub->get(i);
//     //     std::cout << item << std::endl;
//     // }

//     // delete sub;

// }

// void test_static_gen() {
//     Sequence<int>* a = new Array_Sequence<int>;
//     for (int i = 0; i <= 10; ++i) {
//         a->append(i);
//     }

//     Lazy_Sequence<int> l(a);

//     int i = 0;
//     while(l.has_next() && i < 100){
//         int item = l.get(i++);
//         // std::cout << item << std::endl;
//     }

//     // Lazy_Sequence<int>* sub = l.get_subsequence(1, 5);
//     // std::cout << sub->to_string() << std:: endl;

//     // i = 0;
//     // while(sub->has_next() && i < 100){
//     //     int item = sub->get(i++);
//     //     std::cout << item << std::endl;
//     // }

//     // delete sub;

//     // delete a;
// }

void test_append() {
    // Sequence<int>* a = new Array_Sequence<int>;
    // for (int i = 2; i <= 15; ++i) {
    //     a->append(i);
    // }

    Array_Sequence<int> a = Array_Sequence<int>();
    for (int i = -10; i <= -8; ++i) {
        a.append(i);
    }

    Array_Sequence<int> ab = Array_Sequence<int>();
    for (int i = 0; i <= 1; ++i) {
        ab.append(i);
    }

    auto l = Lazy_Sequence<int>::create(ab, 2, fib);
    auto l2 = Lazy_Sequence<int>::create(a);
    auto l3 = l->where(where_div2);

    int i = 0;

    while(l->has_next() && i < 10){
        int item = l->get(i++);
        std::cout << item << std::endl;
    }

        std::cout << std::endl;

    i = 0;
    while(l3->has_next() && i < 10){
        int item = l3->get(i++);
        std::cout << item << std::endl;
    }

    // int item = l3->get(i++);
    // std::cout << item << std::endl;
    

    // delete a;
    // delete b;
}

void test_cardinal() {

    Cardinal val = 0;
    if (!val.is_infinite()) {
        std::cout << val.get_value() << std::endl;
    }

    // std::cout << val << std::endl;
    
}





int main() {
    // try {

        // test_copy_sequence();
        // test_operator();
        // test_lazy_seq();
        // test_static_gen();
        test_append();
        // test_cardinal();

    // } catch(const out_of_range& e){
    //     std::cout << e.what() << std::endl; 
    // }  catch(const data_is_null& e){
    //     std::cout << e.what() << std::endl; 
    // }

    return 0;
}