#include "IntegerInput.hpp"
#include <iostream>
#include <limits>

int get_integer_input(const std::string& prompt) {
    int value;
    bool valid_input = false;
    
    while (!valid_input) {
        std::cout << prompt;
        std::cin >> value;
        
        if (std::cin.fail()) {
            std::cout << "Wrong input" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            valid_input = true;
        }
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}