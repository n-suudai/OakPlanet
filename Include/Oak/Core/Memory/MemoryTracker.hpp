
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#if OAK_USE_HEAP_TRACKING

#include "Oak/Platform/Thread/CriticalSection.hpp"
#include "Oak/Core/Memory/AllocateConfig.hpp"
#include "Oak/Platform/STL/Container.hpp"

namespace Oak
{

class Heap;

struct Allocation;

namespace Detail
{

// for tracking
class STLMapAllocator
{
public:
    static inline Void* AllocateBytesAligned(SizeT bytes, SizeT alignment,
                                             const Char*, Int32, const Char*)
    {
        return AllocatePolicy::AllocateBytesAlignedForTracking(bytes,
                                                               alignment);
    }

    static inline Void DeallocateBytesAligned(Void* pBlock, SizeT alignment)
    {
        AllocatePolicy::DeallocateBytesAlignedForTracking(pBlock, alignment);
    }
};

} // namespace Detail

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
    typedef STL::unordered_map<Void*, Allocation*, std::hash<Void*>,
                               std::equal_to<Void*>,
                               Detail::STLMapAllocator> AllocationMap;

    CriticalSection m_protection;
    AllocationMap m_allocations;
    SizeT m_nextAllocationBookmark;
};

} // namespace Oak

#endif // OAK_USE_HEAP_TRACKING
