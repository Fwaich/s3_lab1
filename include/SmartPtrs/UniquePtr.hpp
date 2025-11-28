#pragma once
#include <type_traits>

template<class T>
class Unique_Ptr 
{
private:
    T* ptr_;

public:
    explicit Unique_Ptr(T* ptr = nullptr) noexcept : ptr_(ptr) {};

    template<typename U>
    explicit Unique_Ptr(U* ptr) noexcept : ptr_(ptr) {
        static_assert(std::is_convertible_v<U*, T*>, 
                     "U* must be convertible to T*");
    }

    Unique_Ptr(const Unique_Ptr&) = delete;
    Unique_Ptr& operator=(const Unique_Ptr&) = delete;

    Unique_Ptr(Unique_Ptr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    template<typename U>
    Unique_Ptr(Unique_Ptr<U>&& other) noexcept : ptr_(other.release()) {
        static_assert(std::is_base_of_v<T, U>, 
                     "U must be derived from T");
    }

    ~Unique_Ptr() {
        delete ptr_;
    }

    T* get() const noexcept {
        return ptr_;
    }

    T* release() noexcept {
        T* tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }

    void reset(T* ptr = nullptr) noexcept {
        if (ptr == ptr_) return;

        delete ptr_;
        ptr_ = ptr;
    }

    template<typename U>
    void reset(U* ptr = nullptr) noexcept {
        static_assert(std::is_base_of_v<T, U>, 
                     "U must be derived from T");
        
        if (static_cast<T*>(ptr) == ptr_) return;

        delete ptr_;
        ptr_ = ptr;
    }

    void swap(Unique_Ptr& other) noexcept {
        T* temp = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = temp;
    }

    Unique_Ptr& operator=(Unique_Ptr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    template<typename U>
    Unique_Ptr& operator=(Unique_Ptr<U>&& other) noexcept {
        static_assert(std::is_base_of_v<T, U>, 
                     "U must be derived from T");
        
        reset(other.release());
        return *this;
    }

    T* operator->() const noexcept {
        return ptr_;
    }

    T& operator*() const noexcept {
        return *ptr_;
    }

    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    template<typename U>
    friend class Unique_Ptr;
};

template<class T>
class Unique_Ptr<T[]> 
{
private:
    T* ptr_;

public:
    explicit Unique_Ptr(T* ptr = nullptr) noexcept : ptr_(ptr) {};

    Unique_Ptr(const Unique_Ptr&) = delete;
    Unique_Ptr& operator=(const Unique_Ptr&) = delete;

    Unique_Ptr(Unique_Ptr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    ~Unique_Ptr() {
        delete[] ptr_;
    }
    
    T* get() const noexcept {
        return ptr_;
    }
    
    T* release() noexcept {
        T* tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }
    
    void reset(T* ptr = nullptr) noexcept {
        if (ptr == ptr_) return;
        
        delete[] ptr_;
        ptr_ = ptr;
    }
    
    void swap(Unique_Ptr& other) noexcept {
        T* temp = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = temp;
    }

    Unique_Ptr& operator=(Unique_Ptr&& other) noexcept {
        if (this != &other) {
            delete[] ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }
    
    T& operator[](size_t index) const {
        return ptr_[index];
    }
    
    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

};

template<typename T, typename... Args>
Unique_Ptr<T> make_unique(Args&&... args) {
    return Unique_Ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
Unique_Ptr<T[]> make_unique(size_t size) {
    return Unique_Ptr<T[]>(new T[size]);
}