#pragma once
#include <type_traits>
#include "EnableSharedFromThis.hpp"

template<class T> class Weak_Ptr;

template<class T>
class Shared_Ptr 
{
private:
    T* ptr_;
    size_t* ref_count;
    size_t* weak_count;

private:
    void cleanup() {
        if (ref_count) {
            --(*ref_count);

            if (*ref_count == 0) {
                delete ptr_;
                ptr_ = nullptr;

                if (weak_count && *weak_count == 0) {
                    delete ref_count;
                    delete weak_count;
                    ref_count = nullptr;
                    weak_count = nullptr;
                }
            }
        }
    }

    Shared_Ptr(T* p, size_t* rc, size_t* wc) noexcept
        : ptr_(p), ref_count(rc), weak_count(wc) {}

public:

    Shared_Ptr() noexcept
        : ptr_(nullptr), ref_count(nullptr), weak_count(nullptr) {}

    explicit Shared_Ptr(T* ptr) noexcept
        : ptr_(ptr), ref_count(new size_t(1)), weak_count(new size_t(0)) 
    {
        enable_shared_from_this(ptr_);
    }

    template<typename U>
    explicit Shared_Ptr(U* ptr) noexcept
        : ptr_(ptr), ref_count(new size_t(1)), weak_count(new size_t(0))
    {
        static_assert(std::is_convertible_v<U*, T*>,
                     "U* must be convertible to T*");

        enable_shared_from_this(ptr_);
    }

    Shared_Ptr(const Shared_Ptr& other) noexcept
        : ptr_(other.ptr_), ref_count(other.ref_count), weak_count(other.weak_count)
    {
        if (ref_count) ++(*ref_count);
    }

    template<typename U>
    Shared_Ptr(const Shared_Ptr<U>& other) noexcept
        : ptr_(other.ptr_), ref_count(other.ref_count), weak_count(other.weak_count)
    {
        static_assert(std::is_base_of_v<T, U>,
                     "U must be derived from T");
        if (ref_count) ++(*ref_count);
    }

    Shared_Ptr(Shared_Ptr&& other) noexcept
        : ptr_(other.ptr_), ref_count(other.ref_count), weak_count(other.weak_count)
    {
        other.ptr_ = nullptr;
        other.ref_count = nullptr;
        other.weak_count = nullptr;
    }

    template<typename U>
    Shared_Ptr(Shared_Ptr<U>&& other) noexcept
        : ptr_(other.ptr_), ref_count(other.ref_count), weak_count(other.weak_count)
    {
        static_assert(std::is_base_of_v<T, U>,
                     "U must be derived from T");

        other.ptr_ = nullptr;
        other.ref_count = nullptr;
        other.weak_count = nullptr;
    }

    explicit Shared_Ptr(const Weak_Ptr<T>& weak) noexcept
        : ptr_(weak.ptr_), ref_count(weak.ref_count), weak_count(weak.weak_count)
    {
        if (ref_count) ++(*ref_count);
    }


    ~Shared_Ptr() {
        cleanup();
    }

    Shared_Ptr& operator=(const Shared_Ptr& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            ref_count = other.ref_count;
            weak_count = other.weak_count;

            if (ref_count) ++(*ref_count);
        }
        return *this;
    }

    template<typename U>
    Shared_Ptr& operator=(const Shared_Ptr<U>& other) noexcept {
        static_assert(std::is_base_of_v<T, U>,
                     "U must be derived from T");
        cleanup();
        ptr_ = other.ptr_;
        ref_count = other.ref_count;
        weak_count = other.weak_count;
        if (ref_count) ++(*ref_count);
        return *this;
    }

    Shared_Ptr& operator=(Shared_Ptr&& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            ref_count = other.ref_count;
            weak_count = other.weak_count;

            other.ptr_ = nullptr;
            other.ref_count = nullptr;
            other.weak_count = nullptr;
        }
        return *this;
    }

    template<typename U>
    Shared_Ptr& operator=(Shared_Ptr<U>&& other) noexcept {
        static_assert(std::is_base_of_v<T, U>,
                     "U must be derived from T");
        cleanup();
        ptr_ = other.ptr_;
        ref_count = other.ref_count;
        weak_count = other.weak_count;

        other.ptr_ = nullptr;
        other.ref_count = nullptr;
        other.weak_count = nullptr;

        return *this;
    }

    T* get() const noexcept { return ptr_; }
    T& operator*()  const noexcept { return *ptr_; }
    T* operator->() const noexcept { return ptr_; }

    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    int use_count() const noexcept { return ref_count ? int(*ref_count) : 0; }
    bool unique() const noexcept { return use_count() == 1; }

    void reset(T* ptr = nullptr) noexcept {
        cleanup();
        if (!ptr) {
            ptr_ = nullptr;
            ref_count = nullptr;
            weak_count = nullptr;
        } else {
            ptr_ = ptr;
            ref_count = new size_t(1);
            weak_count = new size_t(0);
        }
    }

    template<typename U>
    void enable_shared_from_this(U* ptr) noexcept {
        if constexpr (std::is_base_of_v<Enable_Shared_From_This<U>, U>) {
            ptr->weak_this = *this;
        }
    }

    template<typename U>
    void reset(U* ptr) noexcept {
        static_assert(std::is_base_of_v<U, T>,
                     "U must be derived from T");
        reset(static_cast<T*>(ptr));
    }

    void swap(Shared_Ptr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(ref_count, other.ref_count);
        std::swap(weak_count, other.weak_count);
    }

    template<typename U> friend class Shared_Ptr;
    template<typename U> friend class Weak_Ptr;
};

template<class T>
class Shared_Ptr<T[]> 
{
private:
    T* ptr_;
    size_t* ref_count;
    size_t* weak_count;

private:
    void cleanup() {
        if (ref_count) {
            --(*ref_count);

            if (*ref_count == 0) {
                delete[] ptr_;
                ptr_ = nullptr;

                if (weak_count && *weak_count == 0) {
                    delete ref_count;
                    delete weak_count;
                    ref_count = nullptr;
                    weak_count = nullptr;
                }
            }
        }
    }

public:
    Shared_Ptr() noexcept
        : ptr_(nullptr), ref_count(nullptr), weak_count(nullptr) {}

    explicit Shared_Ptr(T* ptr) noexcept
        : ptr_(ptr), ref_count(new size_t(1)), weak_count(new size_t(0)) {}

    Shared_Ptr(const Shared_Ptr& other) noexcept
        : ptr_(other.ptr_), ref_count(other.ref_count), weak_count(other.weak_count)
    {
        if (ref_count) ++(*ref_count);
    }

    Shared_Ptr(Shared_Ptr&& other) noexcept
        : ptr_(other.ptr_), ref_count(other.ref_count), weak_count(other.weak_count)
    {
        other.ptr_ = nullptr;
        other.ref_count = nullptr;
        other.weak_count = nullptr;
    }

    ~Shared_Ptr() {
        cleanup();
    }

    Shared_Ptr& operator=(const Shared_Ptr& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            ref_count = other.ref_count;
            weak_count = other.weak_count;
            if (ref_count) ++(*ref_count);
        }
        return *this;
    }

    Shared_Ptr& operator=(Shared_Ptr&& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            ref_count = other.ref_count;
            weak_count = other.weak_count;

            other.ptr_ = nullptr;
            other.ref_count = nullptr;
            other.weak_count = nullptr;
        }
        return *this;
    }

    T* get() const noexcept { return ptr_; }
    T& operator[](size_t i) const { return ptr_[i]; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    int use_count() const noexcept { return ref_count ? int(*ref_count) : 0; }
    bool unique() const noexcept { return use_count() == 1; }

    void reset(T* ptr = nullptr) noexcept {
        cleanup();
        if (!ptr) {
            ptr_ = nullptr;
            ref_count = nullptr;
            weak_count = nullptr;
        } else {
            ptr_ = ptr;
            ref_count = new size_t(1);
            weak_count = new size_t(0);
        }
    }

    void swap(Shared_Ptr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(ref_count, other.ref_count);
        std::swap(weak_count, other.weak_count);
    }

    template<typename U> friend class Weak_Ptr;
};

template<typename T, typename... Args>
Shared_Ptr<T> make_shared(Args&&... args) {
    return Shared_Ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
Shared_Ptr<T[]> make_shared(size_t size) {
    return Shared_Ptr<T[]>(new T[size]);
}

