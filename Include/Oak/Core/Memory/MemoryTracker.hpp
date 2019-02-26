
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#if OAK_USE_HEAP_TRACKING

#include "Oak/Platform/Thread/CriticalSection.hpp"
#include "Oak/Core/Memory/AllocateConfig.hpp"
#include <unordered_map>

namespace Oak
{

namespace Detail
{

// for tracking
template <typename T>
struct AllocatorForTracking
{
    using value_type = T;

    template <typename U>
    struct rebind
    {
        typedef AllocatorForTracking<U> other;
    };

    inline AllocatorForTracking()
    {/* DO_NOTHING */
    }

    template <typename U>
    inline AllocatorForTracking(const AllocatorForTracking<U>&)
    {/* DO_NOTHING */
    }

    inline T* allocate(SizeT count)
    {
        return reinterpret_cast<T*>(
          AllocatePolicy::AllocateBytesAlignedForTracking(sizeof(T) * count,
                                                          alignof(T)));
    }

    inline Void deallocate(T* ptr, SizeT)
    {
        AllocatePolicy::DeallocateBytesAlignedForTracking(
          reinterpret_cast<Void*>(ptr), alignof(T));
    }
};

template <typename T, typename U>
inline Bool operator==(const AllocatorForTracking<T>&,
                       const AllocatorForTracking<U>&)
{
    return true;
}

template <typename Policy, typename T, typename U>
inline Bool operator!=(const AllocatorForTracking<T>&,
                       const AllocatorForTracking<U>&)
{
    return false;
}

} // namespace Detail

class Heap;
struct Allocation;

class MemoryTracker
{
public:
    static MemoryTracker& Get();

    MemoryTracker();

    ~MemoryTracker();

    Allocation* RecordAllocation(Void* pBlock, SizeT bytes, const Char* file,
                                 Int32 line, const Char* function, Heap* pHeap);

    Void RecordDeallocation(Void* pBlock, Heap* pHeap);

    SizeT GetAllocationBookmark() const;

private:
    typedef std::unordered_map<
      Void*, Allocation*, std::hash<Void*>, std::equal_to<Void*>,
      Detail::AllocatorForTracking<std::pair<const Void*, Allocation*>>>
    AllocationMap;

    CriticalSection m_protection;
    AllocationMap m_allocations;
    SizeT m_nextAllocationBookmark;
};

} // namespace Oak

#endif // OAK_USE_HEAP_TRACKING
