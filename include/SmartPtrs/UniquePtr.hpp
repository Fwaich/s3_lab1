#pragma once

template<class T>
class Unique_Ptr 
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

    void swap(Unique_Ptr& other) noexcept {
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

template<typename T, typename... Args>
Unique_Ptr<T> make_unique(Args&&... args) {
    return Unique_Ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
Unique_Ptr<T[]> make_unique(size_t size) {
    return Unique_Ptr<T[]>(new T[size]());
}