
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD

#include "Oak/Platform/AtomicDataTypes.hpp"

namespace Oak
{

class StandardAllocatePolicy
{
public:
    static Void* AllocateBytes(SizeT bytes);

    static Void* AllocateBytesAligned(SizeT bytes, SizeT alignment);

    static Void DeallocateBytes(Void* pBlock);

    static Void DeallocateBytesAligned(Void* pBlock, SizeT alignment);

#if OAK_USE_HEAP_TRACKING

    static Void* AllocateBytesForTracking(SizeT bytes);

    static Void* AllocateBytesAlignedForTracking(SizeT bytes, SizeT alignment);

    static Void DeallocateBytesForTracking(Void* pBlock);

    static Void DeallocateBytesAlignedForTracking(Void* pBlock,
                                                  SizeT alignment);

#endif // OAK_USE_HEAP_TRACKING
};

} // namespace Oak

#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD
