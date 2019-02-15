
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"


#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD


#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Core/Memory/AlignedAllocator.hpp"
#include "Oak/Core/Memory/MemoryTracker.hpp"


namespace Oak {


class StandardAllocatePolicy
{
public:
    static inline Void* AllocateBytes(SizeT bytes, const Char* file, Int32 line, const Char* func)
    {
        Void* pBlock = reinterpret_cast<Void*>(new UInt8[bytes]);

#if OAK_MEMORY_TRACKER
        MemoryTracker::Get().RecordAllocation(pBlock, bytes, 0, file, line, func);
#else
        // aVoid unused params warning
        file = func = "";
        line = 0;
#endif
        return pBlock;
    }

    static inline Void* DeallocateBytes(Void* pBlock)
    {
        // deal with null
        if (!pBlock) { return; }
#if OAK_MEMORY_TRACKER
        MemoryTracker::Get().RecordDeallocation(pBlock);
#endif
        delete[] (reinterpret_cast<UInt8*>(pBlock));
    }
};



template<SizeT Alignment>
class StandardAlignedAllocatePolicy
{
    // compile-time check alignment is available.
    typedef int IsValidAlignment[Alignment <= 128 && ((Alignment & (Alignment - 1)) == 0) ? +1 : -1];
public:
    static inline Void* AllocateBytes(SizeT bytes, const Char* file, Int32 line, const Char* func)
    {
        Void* pBlock = AlignedMemory::Allocate(bytes, Alignment);

#if OAK_MEMORY_TRACKER
        MemoryTracker::Get().RecordAllocation(pBlock, bytes, 0, file, line, func);
#else
        // aVoid unused params warning
        file = func = "";
        line = 0;
#endif
        return pBlock;
    }

    static inline Void DeallocateBytes(Void* pBlock)
    {
        // deal with null
        if (!pBlock) { return; }
#if OAK_MEMORY_TRACKER
        MemoryTracker::Get().RecordDeallocation(pBlock);
#endif
        AlignedMemory::Deallocate(pBlock);
    }
};


} // namespace Oak


#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD

