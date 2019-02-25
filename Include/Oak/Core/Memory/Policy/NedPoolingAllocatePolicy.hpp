
#pragma once

#include "Oak/Core/Memory/MemoryConfig.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NEDPOOLING

#include "Oak/Platform/AtomicDataTypes.hpp"
#include "Oak/Platform/PlatformDetection.hpp"

namespace Oak
{

class NedPoolingAllocatePolicy
{
public:
    static DECL_MALLOC Void* AllocateBytes(SizeT bytes);

    static DECL_MALLOC Void* AllocateBytesAligned(SizeT bytes, SizeT alignment);

    static Void DeallocateBytes(Void* pBlock);

    static Void DeallocateBytesAligned(Void* pBlock, SizeT alignment);

#if OAK_USE_HEAP_TRACKING

    static DECL_MALLOC Void* AllocateBytesForTracking(SizeT bytes);

    static DECL_MALLOC Void* AllocateBytesAlignedForTracking(SizeT bytes,
                                                             SizeT alignment);

    static Void DeallocateBytesForTracking(Void* pBlock);

    static Void DeallocateBytesAlignedForTracking(Void* pBlock,
                                                  SizeT alignment);

#endif // OAK_USE_HEAP_TRACKING
};

} // namespace Oak

#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NEDPOOLING
