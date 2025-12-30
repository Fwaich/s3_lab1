#pragma once
#include "SharedPtr.hpp"

template <typename T> class Shared_Ptr;
class ControlBlock;

template<class T>
class Weak_Ptr {
private:
    T* ptr = nullptr;
    ControlBlock* control = nullptr;

private:
    void release() noexcept {
        if (!control) return;

        control->decrease_weak();

        if (!control->has_strong() && !control->has_weak()) {
            delete control;
        }

        ptr = nullptr;
        control = nullptr;
    }

public:
    /* constructors */

    Weak_Ptr() noexcept = default;

    Weak_Ptr(const Shared_Ptr<T>& shared) noexcept
        : ptr(shared.ptr), control(shared.control)
    {
        if (control) {
            control->increase_weak();
        }
    }

    Weak_Ptr(const Weak_Ptr& other) noexcept
        : ptr(other.ptr), control(other.control)
    {
        if (control) {
            control->increase_weak();
        }
    }

    Weak_Ptr(Weak_Ptr&& other) noexcept
        : ptr(other.ptr), control(other.control)
    {
        other.ptr = nullptr;
        other.control = nullptr;
    }

    /* destructor */

    ~Weak_Ptr() {
        release();
    }

    /* assignment */

    Weak_Ptr& operator=(const Weak_Ptr& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            control = other.control;
            if (control) {
                control->increase_weak();
            }
        }
        return *this;
    }

    Weak_Ptr& operator=(Weak_Ptr&& other) noexcept {
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

    bool expired() const noexcept {
        return !control || !control->has_strong();
    }

    size_t use_count() const noexcept {
        return control ? control->strong() : 0;
    }

    /* modifiers */

    Shared_Ptr<T> lock() const noexcept {
        return expired() ? Shared_Ptr<T>() : Shared_Ptr<T>(*this);
    }

    void reset() noexcept {
        release();
    }

    /* access */

    T* operator->() const noexcept {
        return ptr;
    }

    /* friends */

    template<class U> friend class Shared_Ptr;
};
