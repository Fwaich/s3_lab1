#pragma once

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

    SharedPtr() noexcept : ptr_(nullptr), ref_count(nullptr) {};

    explicit SharedPtr(T* ptr) noexcept : ptr_(ptr), ref_count(new size_t(1)) {};

    SharedPtr(const SharedPtr& other) noexcept 
        : ptr_(other.ptr_), ref_count(other.ref_count) {
        if (ref_count) {
            ++(*ref_count);
        }
    }

    SharedPtr(SharedPtr&& other) noexcept 
        : ptr_(other.ptr_), ref_count(other.ref_count) {
        other.ptr_ = nullptr;
        other.ref_count = nullptr;
    }

    ~SharedPtr() {
        this->cleanup();
    }

    T* get() const noexcept {
        return ptr_;
    }

    void reset(T* ptr = nullptr) noexcept {
        this->cleanup();
        if (!ptr) {
            ptr_ = nullptr;
            ref_count = nullptr;
            return;
        }

        ptr_ = ptr;
        ref_count = new size_t(1);

    }

    void swap(SharedPtr& other) noexcept {
        T* temp_ptr = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = temp_ptr;

        size_t* temp_ref = ref_count;
        ref_count = other.ref_count;
        other.ref_count = temp_ref;
    }

    bool unique() noexcept {
        if (ref_count && (*ref_count) == 1) {
            return true;
        }

        return false;
    }

    int use_count() const noexcept {
        if (ref_count) {
            return *ref_count;
        }

        return 0;
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
    SharedPtr() noexcept : ptr_(nullptr), ref_count(nullptr) {};

    explicit SharedPtr(T* ptr) noexcept : ptr_(ptr), ref_count(new size_t(1)) {};

    SharedPtr(const SharedPtr& other) noexcept 
        : ptr_(other.ptr_), ref_count(other.ref_count) {
        if (ref_count) {
            ++(*ref_count);
        }
    }

    SharedPtr(SharedPtr&& other) noexcept 
        : ptr_(other.ptr_), ref_count(other.ref_count) {
        other.ptr_ = nullptr;
        other.ref_count = nullptr;
    }

    ~SharedPtr() {
        this->cleanup();
    }

    T* get() const noexcept {
        return ptr_;
    }

    void reset(T* ptr = nullptr) noexcept {
        this->cleanup();
        if (!ptr) {
            ptr_ = nullptr;
            ref_count = nullptr;
            return;
        }

        ptr_ = ptr;
        ref_count = new size_t(1);
    }

    void swap(SharedPtr& other) noexcept {
        T* temp_ptr = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = temp_ptr;

        size_t* temp_ref = ref_count;
        ref_count = other.ref_count;
        other.ref_count = temp_ref;
    }

    bool unique() noexcept {
        if (ref_count && (*ref_count) == 1) {
            return true;
        }
        return false;
    }

    int use_count() const noexcept {
        if (ref_count) {
            return *ref_count;
        }
        return 0;
    }

    T& operator[](size_t index) const {
        return ptr_[index];
    }


    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }
};