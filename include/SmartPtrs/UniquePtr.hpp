#pragma once

template<class T>
class UniquePtr 
{
private:
    T* ptr_;

public:
    explicit UniquePtr(T* ptr = nullptr) noexcept : ptr_(ptr) {};

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    ~UniquePtr() {
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

    void swap(UniquePtr& other) noexcept {
        T* temp = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = temp;
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

};

template<class T>
class UniquePtr<T[]> 
{
private:
    T* ptr_;

public:
    explicit UniquePtr(T* ptr = nullptr) noexcept : ptr_(ptr) {};

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    ~UniquePtr() {
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
    
    void swap(UniquePtr& other) noexcept {
        T* temp = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = temp;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset(other.ptr_);
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