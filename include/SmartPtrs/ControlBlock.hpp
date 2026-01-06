#pragma once

class Control_Block {
private:
    size_t strong_refs;
    size_t weak_refs;

public:
    Control_Block(size_t strong, size_t weak)
        : strong_refs(strong), weak_refs(weak) {}

    ~Control_Block() = default;
    
    size_t strong() const noexcept { return strong_refs; }
    size_t weak()   const noexcept { return weak_refs; }

    bool has_strong() const noexcept { return strong_refs != 0; }
    bool has_weak()   const noexcept { return weak_refs != 0; }

    void increase_strong() noexcept { ++strong_refs; }
    void increase_weak()   noexcept { ++weak_refs;   }

    void decrease_strong() noexcept { --strong_refs; }
    void decrease_weak()   noexcept { --weak_refs;   }
};