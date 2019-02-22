

#include "Oak/Core/Memory/Policy/NedAllocatePolicy.hpp"

#if OAK_MEMORY_ALLOCATOR == OAK_MEMORY_ALLOCATOR_NED

#include "Oak/Core/Assert.hpp"
#include "Oak/Core/Bitwise.hpp"
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

Void* NedAllocatePolicyImpl::AllocateBytesAligned(SizeT bytes, SizeT alignment)
{
    OAK_ASSERT(0 < alignment && alignment <= 128 &&
               Bitwise::IsPowerOf2(alignment));

    // default to platform SIMD alignment if none specified
    return nedalloc::nedmemalign(alignment ? alignment : OAK_SIMD_ALIGNMENT,
                                 bytes);
}

Void NedAllocatePolicyImpl::DeallocateBytesAligned(Void* pBlock,
                                                   SizeT alignment)
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
