
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD

#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Core/Memory/AlignedAllocator.hpp"

namespace Oak
{

class StandardAllocatePolicy
{
public:
    static inline Void* AllocateBytes(SizeT bytes)
    {
        return malloc(bytes);
    }

    static inline Void* AllocateBytesAligned(SizeT bytes, SizeT alignment)
    {
        return AlignedMemory::Allocate(bytes, alignment);
    }

    static inline Void DeallocateBytes(Void* pBlock)
    {
        // deal with null
        if (!pBlock)
        {
            return;
        }

        free(pBlock);
    }

    static inline Void DeallocateBytesAligned(Void* pBlock, SizeT alignment)
    {
        alignment;

        // deal with null
        if (!pBlock)
        {
            return;
        }

        AlignedMemory::Deallocate(pBlock);
    }
};

} // namespace Oak

#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD
