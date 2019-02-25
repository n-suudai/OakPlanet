

#include "Oak/Core/Memory/Policy/NedAllocatePolicy.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED

#include "Oak/Core/Assert.hpp"
#include "Oak/Core/Bitwise.hpp"
#include "Oak/ThirdParty/nedmalloc.hpp"
#include <algorithm>

namespace Oak
{

namespace NedAllocatePolicyInternal
{

Void* internalAlloc(SizeT bytes)
{
    return nedalloc::nedmalloc(bytes);
}

Void* internalAllocAligned(SizeT bytes, SizeT alignment)
{
    OAK_ASSERT(0 < alignment && alignment <= 128 &&
               Bitwise::IsPowerOf2(alignment));

    // default to platform SIMD alignment if none specified
    return nedalloc::nedmemalign(alignment ? alignment : OAK_SIMD_ALIGNMENT,
                                 bytes);
}

Void internalFree(Void* pBlock)
{
    // deal with null
    if (!pBlock)
    {
        return;
    }

    nedalloc::nedfree(pBlock);
}

} // namespace NedAllocatePolicyInternal

Void* NedAllocatePolicy::AllocateBytes(SizeT bytes)
{
    return NedAllocatePolicyInternal::internalAlloc(bytes);
}

Void* NedAllocatePolicy::AllocateBytesAligned(SizeT bytes, SizeT alignment)
{
    return NedAllocatePolicyInternal::internalAllocAligned(bytes, alignment);
}

Void NedAllocatePolicy::DeallocateBytes(Void* pBlock)
{
    NedAllocatePolicyInternal::internalFree(pBlock);
}

Void NedAllocatePolicy::DeallocateBytesAligned(Void* pBlock, SizeT alignment)
{
    alignment;

    NedAllocatePolicyInternal::internalFree(pBlock);
}

#if OAK_USE_HEAP_TRACKING

Void* NedAllocatePolicy::AllocateBytesForTracking(SizeT bytes)
{
    return NedAllocatePolicyInternal::internalAlloc(bytes);
}

Void* NedAllocatePolicy::AllocateBytesAlignedForTracking(SizeT bytes,
                                                         SizeT alignment)
{
    return NedAllocatePolicyInternal::internalAllocAligned(bytes, alignment);
}

Void NedAllocatePolicy::DeallocateBytesForTracking(Void* pBlock)
{
    NedAllocatePolicyInternal::internalFree(pBlock);
}

Void NedAllocatePolicy::DeallocateBytesAlignedForTracking(Void* pBlock,
                                                          SizeT alignment)
{
    alignment;
    NedAllocatePolicyInternal::internalFree(pBlock);
}

#endif // OAK_USE_HEAP_TRACKING

} // namespace  Oak

#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED
