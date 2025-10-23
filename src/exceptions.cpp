#include "exceptions.hpp"

const char* out_of_range::what() const noexcept{
    return "index out of data";
}

const char* data_is_null::what() const noexcept{
    return "data isn't intialized";
}

const char* wrong_type::what() const noexcept{
    return "invalid type entered";
}