#pragma once
#include <exception>


class array_out_of_range : public std::exception {
public:
    virtual const char* what() const noexcept override;
};

class data_is_null : public std::exception {
public:
    virtual const char* what() const noexcept override;
};

class wrong_type : public std::exception {
public:
    virtual const char* what() const noexcept override;
};