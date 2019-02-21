

#include "Oak/Core/Memory/Policy/NedAllocatePolicy.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED

#include "Oak/Core/Memory/MemoryTracker.hpp"
#include "Oak/ThirdParty/nedmalloc.hpp"
#include <algorithm>

namespace Oak
{

Void* NedAllocatePolicyImpl::AllocateBytes(SizeT bytes)
{
    return nedalloc::nedmalloc(bytes);
}

Void NedAllocatePolicyImpl::DeallocateBytes(Void* pBlock)
{
    // deal with null
    if (!pBlock)
    {
        return;
    }

    nedalloc::nedfree(pBlock);
}

Void* NedAllocatePolicyImpl::AllocateBytesAligned(SizeT align, SizeT bytes)
{
    // default to platform SIMD alignment if none specified
    return nedalloc::nedmemalign(align ? align : OAK_SIMD_ALIGNMENT, bytes);
}

Void NedAllocatePolicyImpl::DeallocateBytesAligned(SizeT alignment,
                                                   Void* pBlock)
{
    alignment;

    // deal with null
    if (!pBlock)
    {
        return;
    }

    nedalloc::nedfree(pBlock);
}

} // namespace  Oak

#endif // OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED
