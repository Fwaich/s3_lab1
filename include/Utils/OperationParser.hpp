#pragma once
#include <string>
#include <functional>

template <typename T>
class Sequence;

template <typename T>
class Sequence_Parser {
public:
    static std::function<T(const Sequence<T>&)>
    parse(const std::string& expr)
    {
        std::istringstream iss(expr);

        char op;
        T x;

        if (!(iss >> op >> x))
            throw std::invalid_argument("Invalid expression");

        switch (op) {
            case '+':
                return [x](const Sequence<T>& seq) -> T {
                    size_t n = seq.get_size();
                    return seq.get(n - 1) + x;
                };

            case '-':
                return [x](const Sequence<T>& seq) -> T {
                    size_t n = seq.get_size();
                    return seq.get(n - 1) - x;
                };

            case '*':
                return [x](const Sequence<T>& seq) -> T {
                    size_t n = seq.get_size();
                    return seq.get(n - 1) * x;
                };

            case '/':
                return [x](const Sequence<T>& seq) -> T {
                    if (x == T{})
                        throw std::runtime_error("Division by zero");
                    size_t n = seq.get_size();
                    return seq.get(n - 1) / x;
                };

            default:
                throw std::invalid_argument("Unknown operator");
        }
    }
};

template <typename T>
class Map_Parser {
public:
    static std::function<T(const T&)>
    parse(const std::string& expr)
    {
        std::istringstream iss(expr);

        char op;
        T x;

        if (!(iss >> op >> x))
            throw std::invalid_argument("Invalid expression");

        switch (op) {
            case '+':
                return [x](const T& v) {
                    return v + x;
                };

            case '-':
                return [x](const T& v) {
                    return v - x;
                };

            case '*':
                return [x](const T& v) {
                    return v * x;
                };

            case '/':
                return [x](const T& v) {
                    if (x == T{})
                        throw std::runtime_error("Division by zero");
                    return v / x;
                };

            default:
                throw std::invalid_argument("Unknown operator");
        }
    }
};

template <typename T>
class Where_Parser {
public:
    static std::function<bool(const T&)> parse(const std::string& expr) {
        std::istringstream iss(expr);

        std::string op;
        T x;

        if (!(iss >> op >> x))
            throw std::invalid_argument("Invalid expression");

        if (op == ">") return [x](const T& v) { return v > x; };
        if (op == "<") return [x](const T& v) { return v < x; };
        if (op == ">=") return [x](const T& v) { return v >= x; };
        if (op == "<=") return [x](const T& v) { return v <= x; };
        if (op == "==") return [x](const T& v) { return v == x; };
        if (op == "!=") return [x](const T& v) { return v != x; };

        throw std::invalid_argument("Unknown operator for where");
    }
};


