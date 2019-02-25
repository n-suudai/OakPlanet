
#include "Oak/Core/Memory/Policy/StandardAllocatePolicy.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD

#include "Oak/Core/Memory/AlignedAllocator.hpp"
#include <malloc.h>

namespace Oak
{

Void* StandardAllocatePolicy::AllocateBytes(SizeT bytes)
{
    return malloc(bytes);
}

Void* StandardAllocatePolicy::AllocateBytesAligned(SizeT bytes, SizeT alignment)
{
    return AlignedMemory::Allocate(bytes, alignment);
}

Void StandardAllocatePolicy::DeallocateBytes(Void* pBlock)
{
    // deal with null
    if (!pBlock)
    {
        return;
    }

    free(pBlock);
}

Void StandardAllocatePolicy::DeallocateBytesAligned(Void* pBlock,
                                                    SizeT alignment)
{
    alignment;

    // deal with null
    if (!pBlock)
    {
        return;
    }

    AlignedMemory::Deallocate(pBlock);
}

#if OAK_USE_HEAP_TRACKING

Void* StandardAllocatePolicy::AllocateBytesForTracking(SizeT bytes)
{
    return malloc(bytes);
}

Void* StandardAllocatePolicy::AllocateBytesAlignedForTracking(SizeT bytes,
                                                              SizeT alignment)
{
    return AlignedMemory::Allocate(bytes, alignment);
}

Void StandardAllocatePolicy::DeallocateBytesForTracking(Void* pBlock)
{
    // deal with null
    if (!pBlock)
    {
        return;
    }

    free(pBlock);
}

Void StandardAllocatePolicy::DeallocateBytesAlignedForTracking(Void* pBlock,
                                                               SizeT alignment)
{
    alignment;

    // deal with null
    if (!pBlock)
    {
        return;
    }

    AlignedMemory::Deallocate(pBlock);
}

#endif // OAK_USE_HEAP_TRACKING

} // namespace Oak

#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_STD
