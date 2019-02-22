
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"
#include <malloc.h>

namespace Oak
{
namespace STL
{

class StandardAllocator
{
public:
    static Void* AllocateBytes(SizeT bytes, SizeT alignment, const Char*, Int32,
                               const Char*)
    {
        return _mm_malloc(bytes, alignment);
    }

    static Void DeallocateBytes(Void* pBlock)
    {
        _mm_free(pBlock);
    }
};

template <typename T, typename Allocator = StandardAllocator>
struct StdAllocator
{
    using value_type = T;

    template <typename U>
    struct rebind
    {
        typedef StdAllocator<U, Allocator> other;
    };

    StdAllocator()
    {/* DO_NOTHING */
    }

    template <typename U>
    StdAllocator(const StdAllocator<U, Allocator>&)
    {/* DO_NOTHING */
    }

    T* allocate(SizeT count)
    {
        return reinterpret_cast<T*>(Allocator::AllocateBytesAligned(
          sizeof(T) * count, alignof(T), __FILE__, __LINE__, __FUNCTION__));
    }

    Void deallocate(T* ptr, SizeT)
    {
        Allocator::DeallocateBytesAligned(reinterpret_cast<Void*>(ptr),
                                          alignof(T));
    }
};

template <typename T, typename U, typename Allocator>
Bool operator==(const StdAllocator<T, Allocator>&,
                const StdAllocator<U, Allocator>&)
{
    return true;
}

template <typename T, typename U, typename Allocator>
Bool operator!=(const StdAllocator<T, Allocator>&,
                const StdAllocator<U, Allocator>&)
{
    return false;
}

} // namespace STL
} // namespace Oak
