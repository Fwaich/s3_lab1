#pragma once
#include <type_traits>
#include <utility>
#include <cstddef>
#include "EnableSharedFromThis.hpp"
#include "ControlBlock.hpp"

template<class T> class Weak_Ptr;

template<class T>
class Shared_Ptr {
private:
    T* ptr = nullptr;
    Control_Block* control = nullptr;

private:
    void release() noexcept {
        if (!control) return;

        control->decrease_strong();
        bool no_strong = !control->has_strong();
        bool no_weak = !control->has_weak();

        if (no_strong) {
            delete ptr;
            
            if (no_strong && no_weak) {
                delete control;
            }           
        }
        ptr = nullptr;
        control = nullptr;
    }

    template<typename U>
    void enable_shared_from_this(U* p) noexcept {
        if constexpr (std::is_base_of_v<Enable_Shared_From_This<U>, U>) {
            if (!p->weak_this.control) {
                p->weak_this.ptr = p;
                p->weak_this.control = control;
                control->increase_weak();
            }
        }
    }

public:

    Shared_Ptr() noexcept = default;

    explicit Shared_Ptr(T* p)
        : ptr(p), control(nullptr)
    {
        if (p) {
            control = new Control_Block(1, 0);
            enable_shared_from_this(p);
        }
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
        if (control && control->has_strong()) {
            control->increase_strong();
        } else {
            ptr = nullptr;
            control = nullptr;
        }
    }


    ~Shared_Ptr() {
        release();
    }


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


    T& operator*()  const noexcept { return *ptr; } //без проверки как в сырых указателях
    T* get() const noexcept { return ptr; }
    T* operator->() const noexcept { return ptr; }

    explicit operator bool() const noexcept { return ptr != nullptr; }

    size_t use_count() const noexcept {
        return control ? control->strong() : 0;
    }

    bool unique() const noexcept {
        return use_count() == 1;
    }


    void reset(T* p = nullptr) {
        release();
        if (p) {
            ptr = p;
            control = new Control_Block(1, 0);
            enable_shared_from_this(p);
        }
    }

    void swap(Shared_Ptr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(control, other.control);
    }


    template<class U> friend class Shared_Ptr;
    template<class U> friend class Weak_Ptr;
};


namespace my {

    template<typename T, typename... Args>
    Shared_Ptr<T> make_shared(Args&&... args) {
        return Shared_Ptr<T>(new T(std::forward<Args>(args)...));
    }

}