#include "Sequnces.hpp"
#include "UniquePtr.hpp"
#include "SharedPtr.hpp"
#include "ReadOnlyStream.hpp"
#include "RunningMedian.hpp"
#include "OperationParser.hpp"
#include "BinaryTree.hpp"
#include "LazyInit.hpp"
#include <iostream>
#include <limits>

int main() {
    
    auto lazy_seq = init_new_lazy_seq();
    auto stream = my::make_shared<Lazy_Read_Only_Stream<int>>(lazy_seq);
    auto lazy_stream = Lazy_Sequence<int>::create(my::make_unique<Stream_Generator<int>>(stream));
    Running_Median<int> median;
    
    std::cout << std::endl;
    
    bool changed = false;
    size_t current_index = 0;
    bool running = true;
    while (running) {
        try {

            std::cout << "Abilities:\n"
                      << "1.Append\n"
                      << "2.Prepend\n"
                      << "3.Insert At\n"
                      << "4.Map\n"
                      << "5.Where\n"
    
                      << "6.Read next\n"
                      << "7.Read next N\n"
                      << "8.Reset index\n"
                      << "9.Show statistics\n"
                      << "0.Exit\n";
    
            std::cout << "Your choice: ";
    
            int option;
            std::cin >> option;
    
            switch (option) {
                case 1: {
                    auto appendable = init_new_lazy_seq();
                    lazy_stream = lazy_stream->append(appendable);
                    std::cout << "Succes append\n";
                    changed = true;
                    break;
                }
    
                case 2: {
                    auto prepandable = init_new_lazy_seq();
                    lazy_stream = lazy_stream->prepend(prepandable);
                    std::cout << "Succes prepend\n";
                    changed = true;
                    break;
                }
    
                case 3: {
                    std::cout << "Enter Index: ";
                    int index;
                    std::cin >> index;
    
                    auto insertable = init_new_lazy_seq();
                    lazy_stream = lazy_stream->insert_at(index, insertable);
                    std::cout << "Succes insert\n";
                    changed = true;
                    break;
                }

                case 4: {
                    std::cout << "Enter  +,-,/,*  x:\n";

                    std::string operation;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, operation);

                    auto func = Map_Parser<int>::parse(operation);
                    lazy_stream = lazy_stream->map(func);
                }

                case 5: {
                    std::cout << "Enter  >,>=,<,<=,==,!=  x:\n";
                    std::string operation;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, operation);

                    auto func = Where_Parser<int>::parse(operation);
                    lazy_stream = lazy_stream->where(func);
                }
    
                case 6: {
                    std::cout << "Readed:\n";

                    int element = lazy_stream->get(current_index++);
                    median.add(element);

                    std::cout << "[" << current_index - 1 << "]: ";
                    std::cout << element << std::endl;
                    break;
                }
    
                case 7: {
                    std::cout << "Enter N: ";
                    int elements_count;
                    std::cin >> elements_count;
    
                    int i = 0;
                    std::cout << "Readed:\n";
                    while (lazy_stream->has_next() && i < elements_count) {
                        int element = lazy_stream->get(current_index++);
                        median.add(element);
                        i++;

                        std::cout << "[" << current_index - 1 << "]: ";
                        std::cout << element << std::endl;
                    }

                    if (i < elements_count) 
                        std::cout << "Lazy Sequence have only " << i << " elements\n";
                    break;
                }

                case 8:
                    current_index = 0;
                    std::cout << "Current index: 0\n";
                    break;

                case 9: {
                    if (changed) {
                        changed = false;
                        median.clear();

                        int i = 0;
                        while (lazy_stream->has_next() && i <= current_index) {
                            median.add(lazy_stream->get(i++));
                        }
                    }

                    std::cout << "Median: ";
                    std::cout << median.get_median() << std::endl;
                    break;
                }
    
                case 0:
                    running = false;
                    break;
    
    
                default:
                    std::cout << "No such option\n";
                    break;
            }
    
            std::cout << std::endl;

        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << std::endl;
        }

    }
    
    return 0;
}




//  int main() {
//     Running_Median<int> a;

//     a.add(5);
//     a.add(4);
//     a.add(3);
//     a.add(10);
//     a.add(2);
    

//     std::cout << "Size:" << std::endl;
//     std::cout << a.get_median() << std::endl;
    


//     return 0;
//  }