#pragma once
#include "SharedPtr.hpp"
#include "ControlBlock.hpp"

template <typename T> class Shared_Ptr;

template<class T>
class Weak_Ptr {
private:
    T* ptr = nullptr;
    Control_Block* control = nullptr;

private:
    void release() noexcept {
        if (!control) return;

        if (control->has_weak()) control->decrease_weak(); //всегда выполняется (еще подумать)

        if (!control->has_strong() && !control->has_weak()) {
            delete control;
        }

        ptr = nullptr;
        control = nullptr;
    }

public:

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


    ~Weak_Ptr() {
        release();
    }


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


    bool expired() const noexcept {
        return !control || !control->has_strong();
    }

    size_t use_count() const noexcept {
        return control ? control->strong() : 0;
    }


    Shared_Ptr<T> lock() const noexcept {
        return expired() ? Shared_Ptr<T>() : Shared_Ptr<T>(*this);
    }

    void reset() noexcept {
        release();
    }


    T* operator->() const noexcept {
        return ptr; //нужно проверять есть ли ptr
        //может возаращать option
    }


    template<class U> friend class Shared_Ptr;
};
