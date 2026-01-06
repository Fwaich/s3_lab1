#include "LazyInit.hpp"
#include "IntegerInput.hpp"
#include <iostream>
#include <limits>

Shared_Ptr<Lazy_Sequence<int>> init_by_prod_func() {
    std::cout << std::endl;

    bool running = true;
    while (running) {

        try {

            std::cout << "Chose function:\n"
                      << "1. Fibonachi\n"
                      << "2. Last  +,-,/,*  x\n"
                    ;
        
            int init_option = get_integer_input("Your choice: ");
            switch(init_option) {
                case 1: {
                    auto fib = [](const Sequence<int>& seq) -> int {
                        size_t size = seq.get_size();
                        return seq.get(size - 1) + seq.get(size - 2);
                    };
        
                    Array_Sequence<int> start;
                    start.append(0);
                    start.append(1);
                    return Lazy_Sequence<int>::create(start, 2, fib);
                }
    
                case 2: {
                    int value = get_integer_input("Enter start value: ");
                    Array_Sequence<int> start_seq;
                    start_seq.append(value);
    
                    std::cout << "Enter  +,-,/,*  x:\n";
                    std::string operation;
                    std::getline(std::cin, operation);

                    auto func = Sequence_Parser<int>::parse(operation);
                    return Lazy_Sequence<int>::create(start_seq, 1, func);
                }
        
                default:
                    std::cout << "No such option\n";
                    break;
            }

        } catch(const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        } catch(const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
        }

        std::cout << std::endl;

    }

    return Lazy_Sequence<int>::create();
}


Shared_Ptr<Lazy_Sequence<int>> init_by_sequence() {
    int count = get_integer_input("Enter count: ");

    Array_Sequence<int> sequence;
    for (int i = 0; i < count; i++) {
        int element = get_integer_input("");
        sequence.append(element);
    }

    return Lazy_Sequence<int>::create(sequence);
}


Shared_Ptr<Lazy_Sequence<int>> init_new_lazy_seq() {
    std::cout << std::endl;

    bool initializating = true;
    while (initializating) {
        std::cout << "Init Lazy Sequence\n"
                  << "1. By producing func\n"
                  << "2. By sequence\n"
        ;

        int option = get_integer_input("Your choice: ");

        switch (option) {
            case 1:
                return init_by_prod_func();

            case 2:
                return init_by_sequence();

            default:
                std::cout << "No such option\n";
                break;
        }

        std::cout << std::endl;
    }

    return Lazy_Sequence<int>::create();
}