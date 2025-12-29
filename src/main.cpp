#include "Sequnces.hpp"
#include "UniquePtr.hpp"
#include "SharedPtr.hpp"
#include "ReadOnlyStream.hpp"
#include "NumberStatistics.hpp"
#include <iostream>


int main() {
    try {

        std::cout << "=== LazySequence Stream Number_Statistics ===\n";

        Array_Sequence<int> start;
        start.append(0);
        start.append(1);

        auto fib = [](const Array_Sequence<int>& seq) -> int {
            size_t n = seq.get_size();
            return seq.get(n - 1) + seq.get(n - 2);
        };

        auto lazy = Lazy_Sequence<int>::create(start, 2, fib);
        Lazy_Read_Only_Stream<int> stream(lazy);

        Number_Statistics<int> stats;

        bool running = true;
        while (running) {
            std::cout << "\nMenu:\n"
                      << "1 - Read next element\n"
                      << "2 - Read N elements\n"
                      << "3 - Apply map (*2)\n"
                      << "4 - Apply filter (even only)\n"
                      << "5 - Print statistics\n"
                      << "6 - Reset stream\n"
                      << "0 - Exit\n"
                      << ">> ";

            int choice;
            std::cin >> choice;

            switch (choice) {
            case 1: {
                int value = stream.read();
                stats.consume(value);
                std::cout << "Read: " << value << "\n";
                break;
            }

            case 2: {
                size_t n;
                std::cout << "How many? ";
                std::cin >> n;

                for (size_t i = 0; i < n; ++i) {
                    int value = stream.read();
                    stats.consume(value);
                    std::cout << value << " ";
                }
                std::cout << "\n";
                break;
            }

            case 3: {
                lazy = lazy->map<int>([](const int& x) {
                    return x * 2;
                });
                stream = Lazy_Read_Only_Stream<int>(lazy);
                std::cout << "Map (*2) applied\n";
                break;
            }

            case 4: {
                lazy = lazy->where([](int x) {
                    return x % 2 == 0;
                });
                stream = Lazy_Read_Only_Stream<int>(lazy);
                std::cout << "Filter (even numbers) applied\n";
                break;
            }

            case 5:
                std::cout << "| Count: " << stats.get_count()
                  << "\n| Sum: " << stats.get_sum()
                  << "\n| Min: " << stats.get_min()
                  << "\n| Max: " << stats.get_max()
                  << "\n| Mean: " << stats.get_mean()
                  << "\n";
                break;

            case 6:
                stream.reset();
                std::cout << "Stream reset\n";
                break;

            case 0:
                running = false;
                break;

            default:
                std::cout << "Invalid option\n";
            }
        }

        std::cout << "Goodbye!\n";
        return 0;

    } catch(const std::out_of_range& e){
        std::cout << e.what() << std::endl; 
    }  catch(const std::runtime_error& e){
        std::cout << e.what() << std::endl; 
    }

    return 0;
}