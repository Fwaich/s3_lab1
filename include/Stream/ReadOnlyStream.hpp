#pragma once

#include "Sequence.hpp"
#include "exceptions.hpp"

template <typename T>
class ReadOnlyStream 
{
private:
    Sequence<T>* data;
    size_t pos;

public:

    ReadOnlyStream(Sequence<T>* seq) : data(seq), pos(0) {};

    ~ReadOnlyStream() {
        delete data;
    }

    bool is_end_off_stream() const {
        return pos >= data->get_size();
    }

    T  read() {
        if (this->is_end_off_stream()) {
            throw out_of_range();
        }
        return data->get(pos++);
    }

    size_t get_position() const {
        return pos;
    }

    void seek(size_t index) {
        if (index >= data->get_size() || index < 0) {
            throw out_of_range();
        }

        pos = index;
    }

    void reset() {
        pos = 0;
    }


    size_t size() {
        return data->get_size();
    }

};