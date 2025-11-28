#pragma once
#include <type_traits>

template<class T>
class SharedPtr 
{
private:
    T* ptr_;
    size_t* ref_count;

private:
    void cleanup() {
        if (ref_count) {
            --(*ref_count);
            if (*ref_count == 0) {
                delete ptr_;
                delete ref_count;
            }
        }
    }

public:

    SharedPtr() noexcept : ptr_(nullptr), ref_count(nullptr) {}

    explicit SharedPtr(T* ptr) noexcept 
        : ptr_(ptr), ref_count(new size_t(1)) {}

    template<typename U>
    explicit SharedPtr(U* ptr) noexcept : ptr_(ptr), ref_count(new size_t(1)) {
        static_assert(std::is_convertible_v<U*, T*>, 
                     "U* must be convertible to T*");
    }

    // copy
    SharedPtr(const SharedPtr& other) noexcept : ptr_(other.ptr_), ref_count(other.ref_count) {
        if (ref_count) ++(*ref_count);
    }

    template<typename U>
    SharedPtr(const SharedPtr<U>& other) noexcept : ptr_(other.ptr_), ref_count(other.ref_count) {
        static_assert(std::is_base_of_v<T, U>, 
                     "U must be derived from T");

        if (ref_count) ++(*ref_count);
    }

    // move
    SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), ref_count(other.ref_count) {
        other.ptr_ = nullptr;
        other.ref_count = nullptr;
    }

    template<typename U>
    SharedPtr(SharedPtr<U>&& other) noexcept : ptr_(other.ptr_), ref_count(other.ref_count) {
        static_assert(std::is_base_of_v<T, U>, 
                     "U must be derived from T");
        other.ptr_ = nullptr;
        other.ref_count = nullptr;
    }

    ~SharedPtr() {
        cleanup();
    }

    // copy assign
    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            ref_count = other.ref_count;
            if (ref_count) ++(*ref_count);
        }
        return *this;
    }

    template<typename U>
    SharedPtr& operator=(const SharedPtr<U>& other) noexcept {
        static_assert(std::is_base_of_v<T, U>, 
                     "U must be derived from T");

        cleanup();
        ptr_ = other.ptr_;
        ref_count = other.ref_count;
        if (ref_count) ++(*ref_count);
        return *this;
    }

    // move assign
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            ref_count = other.ref_count;

            other.ptr_ = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    template<typename U>
    SharedPtr& operator=(SharedPtr<U>&& other) noexcept {
        static_assert(std::is_base_of_v<T, U>, 
                     "U must be derived from T");

        cleanup();
        ptr_ = other.ptr_;
        ref_count = other.ref_count;

        other.ptr_ = nullptr;
        other.ref_count = nullptr;
        return *this;
    }

    // accessors
    T* get() const noexcept { return ptr_; }
    T& operator*() const noexcept { return *ptr_; }
    T* operator->() const noexcept { return ptr_; }

    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    int use_count() const noexcept {
        return ref_count ? int(*ref_count) : 0;
    }

    bool unique() const noexcept {
        return use_count() == 1;
    }

    void reset(T* ptr = nullptr) noexcept {
        cleanup();
        if (!ptr) {
            ptr_ = nullptr;
            ref_count = nullptr;
        } else {
            ptr_ = ptr;
            ref_count = new size_t(1);
        }
    }

    template<typename U>
    void reset(U* ptr) noexcept {
        static_assert(std::is_base_of_v<T, U>, 
                     "U must be derived from T");
        reset(static_cast<T*>(ptr));
    }

    void swap(SharedPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(ref_count, other.ref_count);
    }

    template<typename U>
    friend class SharedPtr;
};

template<class T>
class SharedPtr<T[]> 
{
private:
    T* ptr_;
    size_t* ref_count;

private:
    void cleanup() {
        if (ref_count) {
            --(*ref_count);
            if (*ref_count == 0) {
                delete[] ptr_;
                delete ref_count;
            }
        }
    }

public:
    SharedPtr() noexcept : ptr_(nullptr), ref_count(nullptr) {}

    explicit SharedPtr(T* ptr) noexcept 
        : ptr_(ptr), ref_count(new size_t(1)) {}

    SharedPtr(const SharedPtr& other) noexcept : ptr_(other.ptr_), ref_count(other.ref_count) {
        if (ref_count) ++(*ref_count);
    }

    SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), ref_count(other.ref_count) {
        other.ptr_ = nullptr;
        other.ref_count = nullptr;
    }

    ~SharedPtr() {
        cleanup();
    }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            ref_count = other.ref_count;
            if (ref_count) ++(*ref_count);
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            ref_count = other.ref_count;
            other.ptr_ = nullptr;
            other.ref_count = nullptr;
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
        } else {
            ptr_ = ptr;
            ref_count = new size_t(1);
        }
    }

    void swap(SharedPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(ref_count, other.ref_count);
    }
};


template<typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
SharedPtr<T[]> make_shared(size_t size) {
    return SharedPtr<T[]>(new T[size]);
}
