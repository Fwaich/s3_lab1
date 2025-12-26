#pragma once
#include "SharedPtr.hpp"
#include "WeakPtr.hpp"

template<typename T>
class Enable_Shared_From_This
{
protected:
    mutable Weak_Ptr<T> weak_this;

public:
    Shared_Ptr<T> shared_from_this() {
        return weak_this.lock();
    }

    Shared_Ptr<const T> shared_from_this() const {
        return weak_this.lock();
    }

protected:
    
    ~Enable_Shared_From_This() = default;

    template<typename U>
    friend class Shared_Ptr;
};
