
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
    static inline Void* AllocateBytes(SizeT bytes)
    {
        return reinterpret_cast<Void*>(new UInt8[bytes]);
    }

    static inline Void DeallocateBytes(Void* pBlock)
    {
        // deal with null
        if (!pBlock) { return; }

        delete[] (reinterpret_cast<UInt8*>(pBlock));
    }
};



template<SizeT Alignment>
class StandardAlignedAllocatePolicy
{
    // compile-time check alignment is available.
    typedef int IsValidAlignment[Alignment <= 128 && ((Alignment & (Alignment - 1)) == 0) ? +1 : -1];
public:
    static inline Void* AllocateBytes(SizeT bytes)
    {
        return AlignedMemory::Allocate(bytes, Alignment);
    }

    static inline Void DeallocateBytes(Void* pBlock)
    {
        // deal with null
        if (!pBlock) { return; }

        AlignedMemory::Deallocate(pBlock);
    }
};


} // namespace Oak


#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD

