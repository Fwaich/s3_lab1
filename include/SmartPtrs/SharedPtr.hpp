#pragma once
#include <type_traits>
#include <utility>
#include <cstddef>
#include "EnableSharedFromThis.hpp"

template<class T> class Weak_Ptr;

class ControlBlock {
public:
    ControlBlock()
        : strong_refs(0), weak_refs(0) {}

    ControlBlock(size_t strong, size_t weak)
        : strong_refs(strong), weak_refs(weak) {}

    ~ControlBlock() = default;

public:
    size_t strong() const noexcept { return strong_refs; }
    size_t weak()   const noexcept { return weak_refs; }

    bool has_strong() const noexcept { return strong_refs != 0; }
    bool has_weak()   const noexcept { return weak_refs != 0; }

    void increase_strong() noexcept { ++strong_refs; }
    void increase_weak()   noexcept { ++weak_refs;   }

    void decrease_strong() noexcept { --strong_refs; }
    void decrease_weak()   noexcept { --weak_refs;   }

private:
    size_t strong_refs;
    size_t weak_refs;
};

template<class T> class Weak_Ptr;

template<class T>
class Shared_Ptr {
private:
    T* ptr = nullptr;
    ControlBlock* control = nullptr;

private:
    void release() noexcept {
        if (!control) return;
        control->increase_weak();

        control->decrease_strong();

        if (!control->has_strong()) {
            delete ptr;
            ptr = nullptr;

        }

        control->decrease_weak();
        if (!control->has_weak()) {
            delete control;
        }

        control = nullptr;
    }

    template<typename U>
    void enable_shared_from_this(U* p) noexcept {
        if constexpr (std::is_base_of_v<Enable_Shared_From_This<U>, U>) {
            p->weak_this = *this;
        }
    }

public:
    /* constructors */

    Shared_Ptr() noexcept = default;

    explicit Shared_Ptr(T* p)
        : ptr(p), control(new ControlBlock(1, 0))
    {
        enable_shared_from_this(p);
    }

    Shared_Ptr(const Shared_Ptr& other) noexcept
        : ptr(other.ptr), control(other.control)
    {
        if (control) {
            control->increase_strong();
        }
    }

    template<typename U>
    Shared_Ptr(const Shared_Ptr<U>& other) noexcept
        : ptr(other.ptr), control(other.control)
    {
        static_assert(std::is_base_of_v<T, U>,
                      "U must be derived from T");
        if (control) {
            control->increase_strong();
        }
    }

    Shared_Ptr(Shared_Ptr&& other) noexcept
        : ptr(other.ptr), control(other.control)
    {
        other.ptr = nullptr;
        other.control = nullptr;
    }

    explicit Shared_Ptr(const Weak_Ptr<T>& weak) noexcept
        : ptr(weak.ptr), control(weak.control)
    {
        if (control) {
            control->increase_strong();
        }
    }

    /* destructor */

    ~Shared_Ptr() {
        release();
    }

    /* assignment */

    Shared_Ptr& operator=(const Shared_Ptr& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            control = other.control;
            if (control) {
                control->increase_strong();
            }
        }
        return *this;
    }

    Shared_Ptr& operator=(Shared_Ptr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            control = other.control;
            other.ptr = nullptr;
            other.control = nullptr;
        }
        return *this;
    }

    /* observers */

    T* get() const noexcept { return ptr; }
    T& operator*()  const noexcept { return *ptr; }
    T* operator->() const noexcept { return ptr; }

    explicit operator bool() const noexcept { return ptr != nullptr; }

    size_t use_count() const noexcept {
        return control ? control->strong() : 0;
    }

    bool unique() const noexcept {
        return use_count() == 1;
    }

    /* modifiers */

    void reset(T* p = nullptr) {
        release();
        if (p) {
            ptr = p;
            control = new ControlBlock(1, 0);
            enable_shared_from_this(p);
        }
    }

    void swap(Shared_Ptr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(control, other.control);
    }

    /* friends */

    template<class U> friend class Shared_Ptr;
    template<class U> friend class Weak_Ptr;
};


namespace my {

    template<typename T, typename... Args>
    Shared_Ptr<T> make_shared(Args&&... args) {
        return Shared_Ptr<T>(new T(std::forward<Args>(args)...));
    }
    
    template<typename T>
    Shared_Ptr<T[]> make_shared(size_t size) {
        return Shared_Ptr<T[]>(new T[size]);
    }

}