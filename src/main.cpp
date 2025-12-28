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


void test_append() {
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
    auto l3 = l->prepend(l2);

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

}


int main() {
    try {

        // test_copy_sequence();
        // test_operator();
        // test_lazy_seq();
        // test_static_gen();
        test_append();
        // test_cardinal();

    } catch(const std::out_of_range& e){
        std::cout << e.what() << std::endl; 
    }  catch(const std::runtime_error& e){
        std::cout << e.what() << std::endl; 
    }

    return 0;
}