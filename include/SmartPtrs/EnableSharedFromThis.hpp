#pragma once
#include "SharedPtr.hpp"
#include "WeakPtr.hpp"

template<typename T>
class Enable_Shared_From_This
{
protected:
    Weak_Ptr<T> weak_this;

public:
    Shared_Ptr<T> shared_from_this() {
        return weak_this.lock();
    }

protected:
    
    virtual ~Enable_Shared_From_This() = default; //виртуальный

    template<typename U>
    friend class Shared_Ptr;
};
