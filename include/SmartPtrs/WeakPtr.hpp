template<class T>
class Weak_Ptr 
{
private:
    T* ptr_;
    size_t* ref_count;
    size_t* weak_count;

public:

    Weak_Ptr() noexcept : ptr_(nullptr), ref_count(nullptr), weak_count(nullptr) {}

    Weak_Ptr(const Shared_Ptr<T>& shared) noexcept
        : ptr_(shared.ptr_), ref_count(shared.ref_count), weak_count(shared.weak_count)
    {
        if (weak_count) ++(*weak_count);
    }

    Weak_Ptr(const Weak_Ptr& other) noexcept
        : ptr_(other.ptr_), ref_count(other.ref_count), weak_count(other.weak_count)
    {
        if (weak_count) ++(*weak_count);
    }

    Weak_Ptr(Weak_Ptr&& other) noexcept
        : ptr_(other.ptr_), ref_count(other.ref_count), weak_count(other.weak_count)
    {
        other.ptr_ = nullptr;
        other.ref_count = nullptr;
        other.weak_count = nullptr;
    }

    ~Weak_Ptr() {
        if (weak_count) {
            --(*weak_count);
            if (*weak_count == 0 && ref_count && *ref_count == 0) {
                delete weak_count;
                delete ref_count;
            }
        }
    }

    Weak_Ptr& operator=(const Weak_Ptr& other) noexcept {
        if (this != &other) {
            this->~Weak_Ptr();
            ptr_ = other.ptr_;
            ref_count = other.ref_count;
            weak_count = other.weak_count;
            if (weak_count) ++(*weak_count);
        }
        return *this;
    }

    Weak_Ptr& operator=(Weak_Ptr&& other) noexcept {
        if (this != &other) {
            this->~Weak_Ptr();
            ptr_ = other.ptr_;
            ref_count = other.ref_count;
            weak_count = other.weak_count;

            other.ptr_ = nullptr;
            other.ref_count = nullptr;
            other.weak_count = nullptr;
        }
        return *this;
    }

    T* operator->() const noexcept { return ptr_; }

    bool expired() const noexcept {
        return !ref_count || *ref_count == 0;
    }

    Shared_Ptr<T> lock() const noexcept {
        return expired() ? Shared_Ptr<T>() : Shared_Ptr<T>(*this);
    }

    int use_count() const noexcept {
        return ref_count ? int(*ref_count) : 0;
    }

    template<typename U>
    friend class Shared_Ptr;
};