#pragma once
#include <variant>

enum infinite {
    alephnull = 0
};

class Cardinal {
private:
    using finite = size_t;
    std::variant<finite, infinite> value;
public:

    Cardinal() : value(size_t(0)) {};

    Cardinal(finite val) : value(val) {}

    Cardinal(infinite val) : value(val) {}

    bool is_infinite() const {
        if (value.index() == 1) return true;
        return false;
    }

    finite get_value() const {
        if (is_infinite()) 
            throw std::logic_error("Cardinal is infinite");
        return std::get<finite>(value);
    }

    std::string to_string() const {
        std::ostringstream oss;
        if (is_infinite()) {
            oss << "ℵ₀";
        } else {
            oss << std::get<finite>(value);
        }
        return oss.str();
    } 

    bool operator==(const Cardinal& other) const {
        if (is_infinite() && other.is_infinite()) return true;
        if (is_infinite() != other.is_infinite()) return false;
        return std::get<finite>(value) == std::get<finite>(other.value);
    }

    bool operator<(const Cardinal& other) const {
        if (is_infinite() && !other.is_infinite()) return false;
        if (!is_infinite() && other.is_infinite()) return true;
        if (is_infinite() && other.is_infinite()) return false; 
        return std::get<finite>(value) < std::get<finite>(other.value);
    }

    Cardinal operator+(const Cardinal& other) const {
        if (is_infinite() || other.is_infinite()) return Cardinal(alephnull);
        return Cardinal(std::get<finite>(value) + std::get<finite>(other.value));
    }

};