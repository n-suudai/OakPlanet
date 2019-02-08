
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"
#include <malloc.h>


namespace Oak {
namespace STL {


class StandardAllocator
{
public:
    static Void* MallocDebug(const Char*, UInt64, SizeT bytes, SizeT alignment)
    {
        return _mm_malloc(bytes, alignment);
    }

    static Void Free(Void* pBlock)
    {
        _mm_free(pBlock);
    }
};


template<typename T, typename Allocator = StandardAllocator>
struct StdAllocator
{
    using value_type = T;

    template<typename U>
    struct rebind {
        typedef StdAllocator<U, Allocator> other;
    };

    StdAllocator()
    { /* DO_NOTHING */
    }

    template<typename U>
    StdAllocator(const StdAllocator<U, Allocator>&)
    { /* DO_NOTHING */
    }

    T* allocate(SizeT count)
    {
        return reinterpret_cast<T*>(Allocator::MallocDebug(
            __FILE__,
            __LINE__,
            sizeof(T) * count,
            alignof(T)
        ));
    }

    Void deallocate(T* ptr, SizeT)
    {
        Allocator::Free(
            reinterpret_cast<Void*>(ptr)
        );
    }
};

template<typename T, typename U, typename Allocator>
Bool operator == (const StdAllocator<T, Allocator>&, const StdAllocator<U, Allocator>&)
{
    return true;
}

template<typename T, typename U, typename Allocator>
Bool operator != (const StdAllocator<T, Allocator>&, const StdAllocator<U, Allocator>&)
{
    return false;
}

} // namespace STL
} // namespace Oak

