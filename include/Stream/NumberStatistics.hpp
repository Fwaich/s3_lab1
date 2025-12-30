#pragma once

template <typename T>
class Number_Statistics {
private:
    size_t count = 0;
    T sum = 0;
    T min_val;
    T max_val;
    bool initialized = false;

public:
    void consume(const T& value) {
        if (!initialized) {
            min_val = max_val = value;
            initialized = true;
        } else {
            if (value < min_val) min_val = value;
            if (value > max_val) max_val = value;
        }

        sum += value;
        ++count;
    }

    size_t get_count() const { return count; }

    T get_sum() const { return sum; }

    double get_mean() const { //медиана
        return count == 0 ? 0.0 : (double)sum / count;
    }

    T get_min() const { return min_val; }

    T get_max() const { return max_val; }

};
